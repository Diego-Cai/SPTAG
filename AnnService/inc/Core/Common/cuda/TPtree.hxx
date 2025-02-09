/*
 * Copyright (c) 2019-2020, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Licensed under the MIT License
 */


#ifndef _SPTAG_COMMON_CUDA_TPTREE_H_
#define _SPTAG_COMMON_CUDA_TPTREE_H_

#include<iostream>
#include<queue>
//#include <cuda.h>
#include <limits.h>
#include <curand_kernel.h>
#include "params.h"
#include "Distance.hxx"

//template<typename T, typename KEY_T,typename SUMTYPE, int Dim>
//class TPtree;

/************************************************************************************
 * Structure that defines the memory locations where points/ids are for a leaf node
 ************************************************************************************/
class LeafNode {
  public:
    int size;
    int offset;
};


/************************************************************************************
 * Updates the node association for every points from one level to the next 
 * i.e., point associated with node k will become associated with 2k+1 or 2k+2
 ************************************************************************************/
template<typename T, typename KEY_T, typename SUMTYPE, int Dim, int PART_DIMS>
__global__ void update_node_assignments(Point<T,SUMTYPE,Dim>* points, KEY_T* weights, int* partition_dims, int* node_ids, KEY_T* split_keys, int* node_sizes, int N, int level);

/************************************************************************************
 * Determine the sizes (number of points in) each leaf node and sets leafs.size
 ************************************************************************************/
__global__ void count_leaf_sizes(LeafNode* leafs, int* node_ids, int N, int internal_nodes);


__global__ void check_for_imbalance(int* node_ids, int* node_sizes, int nodes_on_level, int ndoe_start, float* frac_to_move, int balanceFactor);

__global__ void initialize_rands(curandState* states, int iter);

__global__ void rebalance_nodes(int* node_ids, int N, float* frac_to_move, curandState* states);


/************************************************************************************
 * Collect list of all point ids associated with a leaf and puts it in leaf_points array.
 * Also updates leafs.offset
 ************************************************************************************/
__global__ void assign_leaf_points(LeafNode* leafs, int* leaf_points, int* node_ids, int N, int internal_nodes);

__global__ void assign_leaf_points_in_batch(LeafNode* leafs, int* leaf_points, int* node_ids, int N, int internal_nodes, int min_id, int max_id);
__global__ void assign_leaf_points_out_batch(LeafNode* leafs, int* leaf_points, int* node_ids, int N, int internal_nodes, int min_id, int max_id);


/************************************************************************************
 * Set of functions to compute mean to pick dividing hyperplanes
 ************************************************************************************/
template<typename T, typename KEY_T, typename SUMTYPE, int Dim, int PART_DIMS>
__global__ void find_level_sum(Point<T,SUMTYPE,Dim>* points, KEY_T* weights, int* partition_dims, int* node_ids, KEY_T* split_keys, int* node_sizes, int N, int nodes_on_level, int level);

//template<typename KEY_T>
//__global__ void compute_mean(KEY_T* split_keys, int* node_sizes, int num_nodes);

/*****************************************************************************************
* Convert sums to means for each split key
*****************************************************************************************/
template<typename KEY_T>
__global__ void compute_mean(KEY_T* split_keys, int* node_sizes, int num_nodes) {
    for (int i = blockIdx.x*blockDim.x + threadIdx.x; i < num_nodes; i += blockDim.x*gridDim.x) {
        if (node_sizes[i] > 0) {
            split_keys[i] /= ((KEY_T)node_sizes[i]);
            node_sizes[i] = 0;
        }
    }
}


/************************************************************************************
 * Definition of the GPU TPtree structure. 
 * Only contains the nodes and hyperplane definitions that partition the data, as well
 * as indexes into the point array.  Does not contain the data itself.
 **********************************************************************************/
template<typename T, typename KEY_T,typename SUMTYPE, int Dim>
class TPtree {
  public:
// for each level of the tree, contains the dimensions and weights that defines the hyperplane
    int* partition_dims;
    KEY_T* weight_list;

// for each node, defines the value of the partitioning hyperplane.  Laid out in breadth-first order
    KEY_T* split_keys; 

    int* node_ids; // For each point, store which node it belongs to (ends at id of leaf)
    int* node_sizes; // Stores the size (number of points) in each node

    int num_nodes; 
    int num_leaves;
    int levels;
    int N;

    LeafNode* leafs; // size and offset of each leaf node

    int* leaf_points; // IDs of points in each leaf. Only needed if we dont permute.


    /************************************************************************************
     * Initialize the structure and allocated enough memory for everything
     **********************************************************************************/
    __host__ void initialize(int N_, int levels_) {

      long long int tree_mem=0;

      N = N_;
      levels = levels_;
      num_leaves = pow(2,levels);

      CUDA_CHECK(cudaMalloc(&node_ids, (N)*sizeof(int)));
      CUDA_CHECK(cudaMemset(node_ids, 0, N*sizeof(int)));
      tree_mem+= N*sizeof(int);

      num_nodes = (2*num_leaves - 1);

      int num_internals = num_nodes - num_leaves;

      tree_mem+=Dim*sizeof(int);

      // Allocate memory for DIMS weights at each level
      CUDA_CHECK(cudaMalloc(&weight_list, Dim*levels*sizeof(KEY_T)));

      tree_mem+= levels*sizeof(int*) + levels*Dim*sizeof(KEY_T);

      tree_mem+= N*sizeof(int);
      CUDA_CHECK(cudaMalloc(&node_sizes, num_nodes*sizeof(int)));
      CUDA_CHECK(cudaMemset(node_sizes, 0, num_nodes*sizeof(int)));

      CUDA_CHECK(cudaMalloc(&split_keys, num_internals*sizeof(KEY_T)));
      tree_mem+= num_nodes*sizeof(int) + num_internals*sizeof(KEY_T);

      CUDA_CHECK(cudaMalloc(&leafs, num_leaves*sizeof(LeafNode)));
      tree_mem+=num_leaves*sizeof(LeafNode);

      CUDA_CHECK(cudaMalloc(&leaf_points, N*sizeof(int)));
      tree_mem+=N*sizeof(int);

    }

    /***********************************************************
     *  Reset ids and sizes so that memory can be re-used for a new TPtree
     * *********************************************************/
    __host__ void reset() {

      cudaMemset(node_ids, 0, N*sizeof(int));
      cudaMemset(node_sizes, 0, num_nodes*sizeof(int));
      cudaMemset(split_keys, 0.0, num_nodes*sizeof(float));

      LeafNode* h_leafs = new LeafNode[num_leaves];
      for(int i=0; i<num_leaves; ++i) {
        h_leafs[i].size=0;
      }
      CUDA_CHECK(cudaMemcpy(leafs, h_leafs, num_leaves*sizeof(LeafNode), cudaMemcpyHostToDevice));
      delete[] h_leafs;
    }

    __host__ void destroy() {
      cudaFree(node_ids);
      cudaFree(weight_list);
      cudaFree(node_sizes);
      cudaFree(split_keys);
      cudaFree(leafs);
      cudaFree(leaf_points);
    }


    /************************************************************************************
     * Construct the tree.  ** Assumes tree has been initialized and allocated **
     * For each level of the tree, compute the mean for each node and set it as the split_key,
     * then compute, for each element, which child node it belongs to (storing in node_ids)
    ************************************************************************************/
    __host__ void construct_tree(Point<T,SUMTYPE,Dim>* points, int min_id, int max_id) {

      int nodes_on_level=1;
      for(int i=0; i<levels; ++i) {

        find_level_sum<T,KEY_T,SUMTYPE,Dim,Dim><<<BLOCKS,THREADS>>>(points, weight_list, partition_dims, node_ids, split_keys, node_sizes, N, nodes_on_level, i);
        CUDA_CHECK(cudaDeviceSynchronize());

        compute_mean<KEY_T><<<BLOCKS,THREADS>>>(split_keys, node_sizes, num_nodes);
        CUDA_CHECK(cudaDeviceSynchronize());

        update_node_assignments<T,KEY_T,SUMTYPE,Dim,Dim><<<BLOCKS,THREADS>>>(points, weight_list, partition_dims, node_ids, split_keys, node_sizes, N, i);
        CUDA_CHECK(cudaDeviceSynchronize());

        nodes_on_level*=2;
      }
      count_leaf_sizes<<<BLOCKS,THREADS>>>(leafs, node_ids, N, num_nodes-num_leaves);
      CUDA_CHECK(cudaDeviceSynchronize());


      LeafNode* h_leafs = new LeafNode[num_leaves];

      CUDA_CHECK(cudaMemcpy(h_leafs, leafs, num_leaves*sizeof(LeafNode), cudaMemcpyDeviceToHost));

      h_leafs[0].offset=0;
      for(int i=1; i<num_leaves; ++i) {
        h_leafs[i].offset = h_leafs[i-1].offset+h_leafs[i-1].size;
      } 

      for(int i=0; i<num_leaves; ++i)
        h_leafs[i].size=0;


      CUDA_CHECK(cudaMemcpy(leafs, h_leafs, num_leaves*sizeof(LeafNode), cudaMemcpyHostToDevice));
      delete[] h_leafs;

      CUDA_CHECK(cudaDeviceSynchronize());
      assign_leaf_points_in_batch<<<BLOCKS,THREADS>>>(leafs, leaf_points, node_ids, N, num_nodes-num_leaves, min_id, max_id);
      CUDA_CHECK(cudaDeviceSynchronize());
      assign_leaf_points_out_batch<<<BLOCKS,THREADS>>>(leafs, leaf_points, node_ids, N, num_nodes-num_leaves, min_id, max_id);
    }

    /************************************************************************************
    // For debugging purposes
    ************************************************************************************/
    __host__ void print_tree(Point<T,SUMTYPE,Dim>* points) {
      printf("nodes:%d, leaves:%d, levels:%d\n", num_nodes, num_leaves, levels);
      for(int i=0; i<levels; ++i) {
        for(int j=0; j<pow(2,i); ++j) {
          printf("(%d) %0.2f, ", node_sizes[(int)pow(2,i)+j-1], split_keys[(int)pow(2,i)+j-1]);
        }
        printf("\n");
      }
    }
};

template<typename T, typename KEY_T, typename SUMTYPE, int Dim>
__host__ void create_tptree(TPtree<T,KEY_T,SUMTYPE,Dim>* d_tree, Point<T,SUMTYPE,Dim>* points, int N, int MAX_LEVELS, int min_id, int max_id) {

  KEY_T* h_weights = new KEY_T[d_tree->levels*Dim];
  for(int i=0; i<d_tree->levels*Dim; ++i) {
    h_weights[i] = ((rand()%2)*2)-1;
  }

  CUDA_CHECK(cudaMemcpy(d_tree->weight_list, h_weights, d_tree->levels*Dim*sizeof(KEY_T), cudaMemcpyHostToDevice));
  
  d_tree->construct_tree(points, min_id, max_id);

  delete h_weights;
}


// Construct TPT on each GPU 
template<typename T, typename KEY_T, typename SUMTYPE, int Dim>
__host__ void construct_trees_multigpu(TPtree<T,KEY_T,SUMTYPE,Dim>** d_trees, Point<T,SUMTYPE,Dim>** points, int N, int NUM_GPUS, cudaStream_t* streams, int balanceFactor) {

    int nodes_on_level=1;

    const int RUN_BLOCKS = min(N/THREADS, BLOCKS);
    const int RAND_BLOCKS = min(N/THREADS, 1024); // Use fewer blocks for kernels using random numbers to cut down memory usage

    float** frac_to_move = new float*[NUM_GPUS];
    curandState** states = new curandState*[NUM_GPUS];

    for(int gpuNum=0; gpuNum < NUM_GPUS; ++gpuNum) {
        CUDA_CHECK(cudaSetDevice(gpuNum));
        CUDA_CHECK(cudaMalloc(&frac_to_move[gpuNum], d_trees[0]->num_nodes*sizeof(float)));
        CUDA_CHECK(cudaMalloc(&states[gpuNum], RAND_BLOCKS*THREADS*sizeof(curandState)));
        initialize_rands<<<RAND_BLOCKS,THREADS>>>(states[gpuNum], 0);
    }
  
    for(int i=0; i<d_trees[0]->levels; ++i) {
        for(int gpuNum=0; gpuNum < NUM_GPUS; ++gpuNum) {
            cudaSetDevice(gpuNum);

            find_level_sum<T,KEY_T,SUMTYPE,Dim,Dim><<<RUN_BLOCKS,THREADS,0,streams[gpuNum]>>>(points[gpuNum], d_trees[gpuNum]->weight_list, d_trees[gpuNum]->partition_dims, d_trees[gpuNum]->node_ids, d_trees[gpuNum]->split_keys, d_trees[gpuNum]->node_sizes, N, nodes_on_level, i);

        }

        // Check and rebalance all levels beyond the first (first level has only 1 node)
        if(i > 0) {
            for(int gpuNum=0; gpuNum < NUM_GPUS; ++gpuNum) {
                cudaSetDevice(gpuNum);

                // Compute imbalance factors for each node on level
                check_for_imbalance<<<RUN_BLOCKS,THREADS,0,streams[gpuNum]>>>(d_trees[gpuNum]->node_ids, d_trees[gpuNum]->node_sizes, nodes_on_level, nodes_on_level-1, frac_to_move[gpuNum], balanceFactor);

                // Randomly reassign points to neighboring nodes as needed based on imbalance factor
                rebalance_nodes<<<RAND_BLOCKS,THREADS,0,streams[gpuNum]>>>(d_trees[gpuNum]->node_ids, N, frac_to_move[gpuNum], states[gpuNum]);
            }
        }

        for(int gpuNum=0; gpuNum < NUM_GPUS; ++gpuNum) {
            cudaSetDevice(gpuNum);

            compute_mean<KEY_T><<<RUN_BLOCKS,THREADS,0,streams[gpuNum]>>>(d_trees[gpuNum]->split_keys, d_trees[gpuNum]->node_sizes, d_trees[gpuNum]->num_nodes);

            update_node_assignments<T,KEY_T,SUMTYPE,Dim,Dim><<<RUN_BLOCKS,THREADS,0,streams[gpuNum]>>>(points[gpuNum], d_trees[gpuNum]->weight_list, d_trees[gpuNum]->partition_dims, d_trees[gpuNum]->node_ids, d_trees[gpuNum]->split_keys, d_trees[gpuNum]->node_sizes, N, i);
        }
        nodes_on_level*=2;

    }

    // Free memory used for rebalancing, etc.
    for(int gpuNum=0; gpuNum < NUM_GPUS; ++gpuNum) {
        cudaSetDevice(gpuNum);
        cudaFree(frac_to_move[gpuNum]);
        cudaFree(states[gpuNum]);
    }
    delete[] frac_to_move;
    delete[] states;

    LeafNode* h_leafs = new LeafNode[d_trees[0]->num_leaves];

    for(int gpuNum=0; gpuNum < NUM_GPUS; ++gpuNum) {
        cudaSetDevice(gpuNum);
        count_leaf_sizes<<<RUN_BLOCKS,THREADS,0,streams[gpuNum]>>>(d_trees[gpuNum]->leafs, d_trees[gpuNum]->node_ids, N, d_trees[gpuNum]->num_nodes - d_trees[gpuNum]->num_leaves);

        CUDA_CHECK(cudaMemcpyAsync(h_leafs, d_trees[gpuNum]->leafs, d_trees[gpuNum]->num_leaves*sizeof(LeafNode), cudaMemcpyDeviceToHost, streams[gpuNum]));

        h_leafs[0].offset = 0;
        for(int i=1; i<d_trees[gpuNum]->num_leaves; ++i) {
            h_leafs[i].offset = h_leafs[i-1].offset + h_leafs[i-1].size;
        } 
        for(int i=0; i<d_trees[gpuNum]->num_leaves; ++i) {
          h_leafs[i].size=0;
        }

        CUDA_CHECK(cudaMemcpyAsync(d_trees[gpuNum]->leafs, h_leafs, d_trees[gpuNum]->num_leaves*sizeof(LeafNode), cudaMemcpyHostToDevice, streams[gpuNum]));

        assign_leaf_points_in_batch<<<RUN_BLOCKS,THREADS,0,streams[gpuNum]>>>(d_trees[gpuNum]->leafs, d_trees[gpuNum]->leaf_points, d_trees[gpuNum]->node_ids, N, d_trees[gpuNum]->num_nodes - d_trees[gpuNum]->num_leaves, 0, N);
    }

    delete[] h_leafs;

    for(int gpuNum=0; gpuNum < NUM_GPUS; ++gpuNum) {
        cudaSetDevice(gpuNum);
        assign_leaf_points_out_batch<<<RUN_BLOCKS,THREADS,0,streams[gpuNum]>>>(d_trees[gpuNum]->leafs, d_trees[gpuNum]->leaf_points, d_trees[gpuNum]->node_ids, N, d_trees[gpuNum]->num_nodes - d_trees[gpuNum]->num_leaves, 0, N);
    }
}


template<typename T, typename KEY_T, typename SUMTYPE, int Dim>
__host__ void create_tptree_multigpu(TPtree<T,KEY_T,SUMTYPE,Dim>** d_trees, Point<T,SUMTYPE,Dim>** points, int N, int MAX_LEVELS, int NUM_GPUS, cudaStream_t* streams, int balanceFactor) {

  KEY_T* h_weights = new KEY_T[d_trees[0]->levels*Dim];
  for(int i=0; i<d_trees[0]->levels*Dim; ++i) {
    h_weights[i] = ((rand()%2)*2)-1;
  }

  // Copy random weights to each GPU
  for(int gpuNum=0; gpuNum<NUM_GPUS; ++gpuNum) {
    cudaSetDevice(gpuNum);
    d_trees[gpuNum]->reset();
    CUDA_CHECK(cudaMemcpy(d_trees[gpuNum]->weight_list, h_weights, d_trees[gpuNum]->levels*Dim*sizeof(KEY_T), cudaMemcpyHostToDevice));
  }

  // Build TPT on each GPU  
  construct_trees_multigpu<T,KEY_T,SUMTYPE,Dim>(d_trees, points, N, NUM_GPUS, streams, balanceFactor);

  delete h_weights;
}


/*****************************************************************************************
 * Helper function to calculated the porjected value of point onto the partitioning hyperplane
 *****************************************************************************************/
template<typename T, typename KEY_T, typename SUMTYPE, int Dim, int PART_DIMS>
__device__ KEY_T weighted_val(Point<T,SUMTYPE,Dim> point, KEY_T* weights, int* dims) {
  KEY_T val=0.0;
  for(int i=0; i<PART_DIMS; ++i) {
    val += (weights[i] * (KEY_T)point.coords[i]);
  }
  return val;
}
template<typename T, typename KEY_T, typename SUMTYPE, int Dim, int PART_DIMS>
__device__ KEY_T weighted_val(Point<uint8_t,SUMTYPE,Dim> point, KEY_T* weights, int* dims) {
  KEY_T val=0.0;
  for(int i=0; i<PART_DIMS/4; ++i) {
    val += (weights[i*4] * (unsigned(point.coords[i]) & 0x000000FF));
    val += (weights[i*4+1] * ((unsigned(point.coords[i]) & 0x0000FF00) >> 8));
    val += (weights[i*4+2] * ((unsigned(point.coords[i]) & 0x00FF0000) >> 16));
    val += (weights[i*4+3] * ((unsigned(point.coords[i]) & 0xFF000000) >> 24));
  }
  return val;
}

template<typename T, typename KEY_T, typename SUMTYPE, int Dim, int PART_DIMS>
__device__ KEY_T weighted_val(Point<int8_t,SUMTYPE,Dim> point, KEY_T* weights, int* dims) {

  KEY_T val=0.0;
  for(int i=0; i<PART_DIMS/4; ++i) {
    val += (KEY_T)(weights[i*4] * (int8_t)(point.coords[i] & 0x000000FF));
    val += (KEY_T)(weights[i*4+1] *(int8_t)((point.coords[i] & 0x0000FF00) >> 8));
    val += (KEY_T)(weights[i*4+2] *(int8_t)((point.coords[i] & 0x00FF0000) >> 16));
    val += (KEY_T)(weights[i*4+3] *(int8_t)((point.coords[i] & 0xFF000000) >> 24));
  }
  return val;
}

template<typename T, typename KEY_T, typename SUMTYPE, int Dim, int PART_DIMS>
__global__ void find_level_sum(Point<T,SUMTYPE,Dim>* points, KEY_T* weights, int* partition_dims, int* node_ids, KEY_T* split_keys, int* node_sizes, int N, int nodes_on_level, int level) {
  KEY_T val=0;
  int size = min(N, nodes_on_level*SAMPLES);
  int step = N/size;
  for(int i=blockIdx.x*blockDim.x+threadIdx.x; i<size; i+=blockDim.x*gridDim.x) {
    val = weighted_val<T,KEY_T,SUMTYPE,Dim,PART_DIMS>(points[i], &weights[level*Dim], partition_dims);
    atomicAdd(&split_keys[node_ids[i]], val);
    atomicAdd(&node_sizes[node_ids[i]], 1);
  }
}

/*****************************************************************************************
 * Assign each point to a node of the next level of the tree (either left child or right).
 *****************************************************************************************/
template<typename T, typename KEY_T, typename SUMTYPE, int Dim, int PART_DIMS>
__global__ void update_node_assignments(Point<T,SUMTYPE,Dim>* points, KEY_T* weights, int* partition_dims, int* node_ids, KEY_T* split_keys, int* node_sizes, int N, int level) {
  
  for(int i=blockIdx.x*blockDim.x+threadIdx.x; i<N; i+=blockDim.x*gridDim.x) {
    node_ids[i] = (2*node_ids[i])+1 + (weighted_val<T,KEY_T,SUMTYPE,Dim,PART_DIMS>(points[i],&weights[level*Dim] ,partition_dims) > split_keys[node_ids[i]]);
  }
}

template<typename T, typename KEY_T, typename SUMTYPE, int Dim, int PART_DIMS>
__device__ int searchForLeaf(TPtree<T,KEY_T,SUMTYPE,Dim>* tree, Point<T,SUMTYPE,Dim>* query) {
    int nodeIdx = 0;
    KEY_T* weights;
    for(int i=0; i<tree->levels; i++) {
        weights = &tree->weight_list[i*Dim];

        if(weighted_val<T,KEY_T,SUMTYPE,Dim,PART_DIMS>(*query, weights, tree->partition_dims) <= tree->split_keys[nodeIdx]) {
            nodeIdx = 2*nodeIdx+1;
        }
        else {
            nodeIdx = 2*nodeIdx+2;
        }
    }
    return (nodeIdx - (tree->num_nodes - tree->num_leaves));
}

#endif
