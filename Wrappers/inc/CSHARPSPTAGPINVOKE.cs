//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 4.0.2
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


class CSHARPSPTAGPINVOKE {

  protected class SWIGExceptionHelper {

    public delegate void ExceptionDelegate(string message);
    public delegate void ExceptionArgumentDelegate(string message, string paramName);

    static ExceptionDelegate applicationDelegate = new ExceptionDelegate(SetPendingApplicationException);
    static ExceptionDelegate arithmeticDelegate = new ExceptionDelegate(SetPendingArithmeticException);
    static ExceptionDelegate divideByZeroDelegate = new ExceptionDelegate(SetPendingDivideByZeroException);
    static ExceptionDelegate indexOutOfRangeDelegate = new ExceptionDelegate(SetPendingIndexOutOfRangeException);
    static ExceptionDelegate invalidCastDelegate = new ExceptionDelegate(SetPendingInvalidCastException);
    static ExceptionDelegate invalidOperationDelegate = new ExceptionDelegate(SetPendingInvalidOperationException);
    static ExceptionDelegate ioDelegate = new ExceptionDelegate(SetPendingIOException);
    static ExceptionDelegate nullReferenceDelegate = new ExceptionDelegate(SetPendingNullReferenceException);
    static ExceptionDelegate outOfMemoryDelegate = new ExceptionDelegate(SetPendingOutOfMemoryException);
    static ExceptionDelegate overflowDelegate = new ExceptionDelegate(SetPendingOverflowException);
    static ExceptionDelegate systemDelegate = new ExceptionDelegate(SetPendingSystemException);

    static ExceptionArgumentDelegate argumentDelegate = new ExceptionArgumentDelegate(SetPendingArgumentException);
    static ExceptionArgumentDelegate argumentNullDelegate = new ExceptionArgumentDelegate(SetPendingArgumentNullException);
    static ExceptionArgumentDelegate argumentOutOfRangeDelegate = new ExceptionArgumentDelegate(SetPendingArgumentOutOfRangeException);

    [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="SWIGRegisterExceptionCallbacks_CSHARPSPTAG")]
    public static extern void SWIGRegisterExceptionCallbacks_CSHARPSPTAG(
                                ExceptionDelegate applicationDelegate,
                                ExceptionDelegate arithmeticDelegate,
                                ExceptionDelegate divideByZeroDelegate, 
                                ExceptionDelegate indexOutOfRangeDelegate, 
                                ExceptionDelegate invalidCastDelegate,
                                ExceptionDelegate invalidOperationDelegate,
                                ExceptionDelegate ioDelegate,
                                ExceptionDelegate nullReferenceDelegate,
                                ExceptionDelegate outOfMemoryDelegate, 
                                ExceptionDelegate overflowDelegate, 
                                ExceptionDelegate systemExceptionDelegate);

    [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="SWIGRegisterExceptionArgumentCallbacks_CSHARPSPTAG")]
    public static extern void SWIGRegisterExceptionCallbacksArgument_CSHARPSPTAG(
                                ExceptionArgumentDelegate argumentDelegate,
                                ExceptionArgumentDelegate argumentNullDelegate,
                                ExceptionArgumentDelegate argumentOutOfRangeDelegate);

    static void SetPendingApplicationException(string message) {
      SWIGPendingException.Set(new global::System.ApplicationException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingArithmeticException(string message) {
      SWIGPendingException.Set(new global::System.ArithmeticException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingDivideByZeroException(string message) {
      SWIGPendingException.Set(new global::System.DivideByZeroException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingIndexOutOfRangeException(string message) {
      SWIGPendingException.Set(new global::System.IndexOutOfRangeException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingInvalidCastException(string message) {
      SWIGPendingException.Set(new global::System.InvalidCastException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingInvalidOperationException(string message) {
      SWIGPendingException.Set(new global::System.InvalidOperationException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingIOException(string message) {
      SWIGPendingException.Set(new global::System.IO.IOException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingNullReferenceException(string message) {
      SWIGPendingException.Set(new global::System.NullReferenceException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingOutOfMemoryException(string message) {
      SWIGPendingException.Set(new global::System.OutOfMemoryException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingOverflowException(string message) {
      SWIGPendingException.Set(new global::System.OverflowException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingSystemException(string message) {
      SWIGPendingException.Set(new global::System.SystemException(message, SWIGPendingException.Retrieve()));
    }

    static void SetPendingArgumentException(string message, string paramName) {
      SWIGPendingException.Set(new global::System.ArgumentException(message, paramName, SWIGPendingException.Retrieve()));
    }
    static void SetPendingArgumentNullException(string message, string paramName) {
      global::System.Exception e = SWIGPendingException.Retrieve();
      if (e != null) message = message + " Inner Exception: " + e.Message;
      SWIGPendingException.Set(new global::System.ArgumentNullException(paramName, message));
    }
    static void SetPendingArgumentOutOfRangeException(string message, string paramName) {
      global::System.Exception e = SWIGPendingException.Retrieve();
      if (e != null) message = message + " Inner Exception: " + e.Message;
      SWIGPendingException.Set(new global::System.ArgumentOutOfRangeException(paramName, message));
    }

    static SWIGExceptionHelper() {
      SWIGRegisterExceptionCallbacks_CSHARPSPTAG(
                                applicationDelegate,
                                arithmeticDelegate,
                                divideByZeroDelegate,
                                indexOutOfRangeDelegate,
                                invalidCastDelegate,
                                invalidOperationDelegate,
                                ioDelegate,
                                nullReferenceDelegate,
                                outOfMemoryDelegate,
                                overflowDelegate,
                                systemDelegate);

      SWIGRegisterExceptionCallbacksArgument_CSHARPSPTAG(
                                argumentDelegate,
                                argumentNullDelegate,
                                argumentOutOfRangeDelegate);
    }
  }

  protected static SWIGExceptionHelper swigExceptionHelper = new SWIGExceptionHelper();

  public class SWIGPendingException {
    [global::System.ThreadStatic]
    private static global::System.Exception pendingException = null;
    private static int numExceptionsPending = 0;
    private static global::System.Object exceptionsLock = null;

    public static bool Pending {
      get {
        bool pending = false;
        if (numExceptionsPending > 0)
          if (pendingException != null)
            pending = true;
        return pending;
      } 
    }

    public static void Set(global::System.Exception e) {
      if (pendingException != null)
        throw new global::System.ApplicationException("FATAL: An earlier pending exception from unmanaged code was missed and thus not thrown (" + pendingException.ToString() + ")", e);
      pendingException = e;
      lock(exceptionsLock) {
        numExceptionsPending++;
      }
    }

    public static global::System.Exception Retrieve() {
      global::System.Exception e = null;
      if (numExceptionsPending > 0) {
        if (pendingException != null) {
          e = pendingException;
          pendingException = null;
          lock(exceptionsLock) {
            numExceptionsPending--;
          }
        }
      }
      return e;
    }

    static SWIGPendingException() {
      exceptionsLock = new global::System.Object();
    }
  }


  protected class SWIGStringHelper {

    public delegate string SWIGStringDelegate(string message);
    static SWIGStringDelegate stringDelegate = new SWIGStringDelegate(CreateString);

    [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="SWIGRegisterStringCallback_CSHARPSPTAG")]
    public static extern void SWIGRegisterStringCallback_CSHARPSPTAG(SWIGStringDelegate stringDelegate);

    static string CreateString(string cString) {
      return cString;
    }

    static SWIGStringHelper() {
      SWIGRegisterStringCallback_CSHARPSPTAG(stringDelegate);
    }
  }

  static protected SWIGStringHelper swigStringHelper = new SWIGStringHelper();


  static CSHARPSPTAGPINVOKE() {
  }

 
    [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)] 
    public struct WrapperArray 
    { 
        public System.IntPtr _data; 
        public ulong _size;
        public WrapperArray(System.IntPtr in_data, ulong in_size) { _data = in_data; _size = in_size; } 
    } 


  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_deleteArrayOfWrapperArray")]
  public static extern void deleteArrayOfWrapperArray(global::System.IntPtr jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_AnnIndex__SWIG_0")]
  public static extern global::System.IntPtr new_AnnIndex__SWIG_0(int jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_AnnIndex__SWIG_1")]
  public static extern global::System.IntPtr new_AnnIndex__SWIG_1(string jarg1, string jarg2, int jarg3);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_delete_AnnIndex")]
  public static extern void delete_AnnIndex(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_SetBuildParam")]
  public static extern void AnnIndex_SetBuildParam(global::System.Runtime.InteropServices.HandleRef jarg1, string jarg2, string jarg3, string jarg4);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_SetSearchParam")]
  public static extern void AnnIndex_SetSearchParam(global::System.Runtime.InteropServices.HandleRef jarg1, string jarg2, string jarg3, string jarg4);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_LoadQuantizer")]
  public static extern bool AnnIndex_LoadQuantizer(global::System.Runtime.InteropServices.HandleRef jarg1, string jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_SetQuantizerADC")]
  public static extern void AnnIndex_SetQuantizerADC(global::System.Runtime.InteropServices.HandleRef jarg1, bool jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_BuildSPANN")]
  public static extern bool AnnIndex_BuildSPANN(global::System.Runtime.InteropServices.HandleRef jarg1, bool jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_BuildSPANNWithMetaData")]
  public static extern bool AnnIndex_BuildSPANNWithMetaData(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2, int jarg3, bool jarg4, bool jarg5);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_Build")]
  public static extern bool AnnIndex_Build(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2, int jarg3, bool jarg4);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_BuildWithMetaData")]
  public static extern bool AnnIndex_BuildWithMetaData(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2, WrapperArray jarg3, int jarg4, bool jarg5, bool jarg6);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_Search")]
  public static extern WrapperArray AnnIndex_Search(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2, int jarg3);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_SearchWithMetaData")]
  public static extern WrapperArray AnnIndex_SearchWithMetaData(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2, int jarg3);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_BatchSearch")]
  public static extern WrapperArray AnnIndex_BatchSearch(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2, int jarg3, int jarg4, bool jarg5);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_ReadyToServe")]
  public static extern bool AnnIndex_ReadyToServe(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_UpdateIndex")]
  public static extern void AnnIndex_UpdateIndex(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_Save")]
  public static extern bool AnnIndex_Save(global::System.Runtime.InteropServices.HandleRef jarg1, string jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_Add")]
  public static extern bool AnnIndex_Add(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2, int jarg3, bool jarg4);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_AddWithMetaData")]
  public static extern bool AnnIndex_AddWithMetaData(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2, WrapperArray jarg3, int jarg4, bool jarg5, bool jarg6);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_Delete")]
  public static extern bool AnnIndex_Delete(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2, int jarg3);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_DeleteByMetaData")]
  public static extern bool AnnIndex_DeleteByMetaData(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_Load")]
  public static extern global::System.IntPtr AnnIndex_Load(string jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_AnnIndex_Merge")]
  public static extern global::System.IntPtr AnnIndex_Merge(string jarg1, string jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_NodeDistPair_node_set")]
  public static extern void NodeDistPair_node_set(global::System.Runtime.InteropServices.HandleRef jarg1, int jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_NodeDistPair_node_get")]
  public static extern int NodeDistPair_node_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_NodeDistPair_distance_set")]
  public static extern void NodeDistPair_distance_set(global::System.Runtime.InteropServices.HandleRef jarg1, float jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_NodeDistPair_distance_get")]
  public static extern float NodeDistPair_distance_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_NodeDistPair__SWIG_0")]
  public static extern global::System.IntPtr new_NodeDistPair__SWIG_0(int jarg1, float jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_NodeDistPair__SWIG_1")]
  public static extern global::System.IntPtr new_NodeDistPair__SWIG_1(int jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_NodeDistPair__SWIG_2")]
  public static extern global::System.IntPtr new_NodeDistPair__SWIG_2();

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_delete_NodeDistPair")]
  public static extern void delete_NodeDistPair(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_Edge_node_set")]
  public static extern void Edge_node_set(global::System.Runtime.InteropServices.HandleRef jarg1, int jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_Edge_node_get")]
  public static extern int Edge_node_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_Edge_distance_set")]
  public static extern void Edge_distance_set(global::System.Runtime.InteropServices.HandleRef jarg1, float jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_Edge_distance_get")]
  public static extern float Edge_distance_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_Edge_tonode_set")]
  public static extern void Edge_tonode_set(global::System.Runtime.InteropServices.HandleRef jarg1, int jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_Edge_tonode_get")]
  public static extern int Edge_tonode_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_Edge")]
  public static extern global::System.IntPtr new_Edge();

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_delete_Edge")]
  public static extern void delete_Edge(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_EdgeCompare")]
  public static extern global::System.IntPtr new_EdgeCompare();

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_delete_EdgeCompare")]
  public static extern void delete_EdgeCompare(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_BasicResult_VID_set")]
  public static extern void BasicResult_VID_set(global::System.Runtime.InteropServices.HandleRef jarg1, int jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_BasicResult_VID_get")]
  public static extern int BasicResult_VID_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_BasicResult_Dist_set")]
  public static extern void BasicResult_Dist_set(global::System.Runtime.InteropServices.HandleRef jarg1, float jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_BasicResult_Dist_get")]
  public static extern float BasicResult_Dist_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_BasicResult_Meta_set")]
  public static extern void BasicResult_Meta_set(global::System.Runtime.InteropServices.HandleRef jarg1, WrapperArray jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_BasicResult_Meta_get")]
  public static extern WrapperArray BasicResult_Meta_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_BasicResult__SWIG_0")]
  public static extern global::System.IntPtr new_BasicResult__SWIG_0();

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_BasicResult__SWIG_1")]
  public static extern global::System.IntPtr new_BasicResult__SWIG_1(int jarg1, float jarg2);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_new_BasicResult__SWIG_2")]
  public static extern global::System.IntPtr new_BasicResult__SWIG_2(int jarg1, float jarg2, WrapperArray jarg3);

  [global::System.Runtime.InteropServices.DllImport("CSHARPSPTAG", EntryPoint="CSharp_delete_BasicResult")]
  public static extern void delete_BasicResult(global::System.Runtime.InteropServices.HandleRef jarg1);
}
