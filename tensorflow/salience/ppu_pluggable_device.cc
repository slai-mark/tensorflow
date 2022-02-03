/* Copyright 2022 Salience...
==============================================================================*/
#include "tensorflow/core/platform/default/logging.h"
#include "ppu_stream_exec.h"
#include "tensorflow/c/kernels.h"
#include "tensorflow/c/ops.h"

extern "C" {

void GetDeviceCount(const SP_Platform* platform, int* device_count,
                    TF_Status* status) {
  TF_SetStatus(status, TF_OK, "");
  *device_count = 1;
}

void CreateDevice(const SP_Platform* platform, SE_CreateDeviceParams* params,
                  TF_Status* status) {
  TF_SetStatus(status, TF_OK, "");
  params->device->struct_size = {SP_DEVICE_STRUCT_SIZE};
  params->device->hardware_name = "Chip A";
  params->device->device_vendor = "Salience Labs";
  params->device->pci_bus_id = "0000:02:03.4";
  LOG(INFO) << "create device";
}
void DestroyDevice(const SP_Platform* platform, SP_Device* device) {}

int32_t get_numa_node(const SP_Device* device) {
  LOG(INFO) << "get numa";
  return 0x69;
}
    
void CreateDeviceFns(const SP_Platform* platform,
                     SE_CreateDeviceFnsParams* params, TF_Status* status) {
  TF_SetStatus(status, TF_OK, "");
  params->device_fns->struct_size = {SP_DEVICE_FNS_STRUCT_SIZE};
  params->device_fns->get_numa_node = get_numa_node;
}
void DestroyDeviceFns(const SP_Platform* platform, SP_DeviceFns* device_fns) {}

void CreateStreamExecutor(const SP_Platform* platform,
                          SE_CreateStreamExecutorParams* params,
                          TF_Status* status) {
			  
  TF_SetStatus(status, TF_OK, "");
  salience::stream_exec::PopulateStreamExecutor(params->stream_executor);
}

void DestroyStreamExecutor(const SP_Platform* platform, SP_StreamExecutor* se) {
}

void CreateTimerFns(const SP_Platform* platform, SP_TimerFns* timer_fns,
                    TF_Status* status) {
  TF_SetStatus(status, TF_OK, "");
  salience::stream_exec::PopulateTimerFns(timer_fns);
}
void DestroyTimerFns(const SP_Platform* platform, SP_TimerFns* timer_fns) {}

void DestroyPlatform(SP_Platform* platform) {}
void DestroyPlatformFns(SP_PlatformFns* platform_fns) {}

typedef struct AssVarOp {
  TF_DataType input_data_type;
  TF_DataType output_data_type;
  size_t in_size;
  size_t out_size;
} AssVarOp;

static void *AssVarOp_Create(TF_OpKernelConstruction *ctx) {
  printf("AssVarOp create\n");
  auto* kernel = new AssVarOp;

  TF_Status* s = TF_NewStatus();
  TF_OpKernelConstruction_GetAttrType(ctx, "T", &kernel->input_data_type, s);
  printf("get code1 is %d\n", TF_GetCode(s));

//  if (TF_GetCode(s) == TF_OK) {
    TF_OpKernelConstruction_GetAttrType(ctx, "type", &kernel->output_data_type,
                                        s);
//  }
  printf("get code2 is %d\n", TF_GetCode(s));

  
  TF_StringView name = TF_OpKernelConstruction_GetName(ctx);
  printf("name is %s\n", name.data);

  TF_DeleteStatus(s);
  return kernel;
}

static void AssVarOp_Compute(void *k, TF_OpKernelContext *ctx) {
  printf("AssVarOp compute\n");
  int num_inputs;
  int num_outputs;
  int bytes, dims, i;
  char *src, *dst;
  num_inputs = TF_NumInputs(ctx);
  num_outputs = TF_NumOutputs(ctx);
  printf("in %d out %d\n", num_inputs, num_outputs);
  
  TF_Tensor *input = nullptr, *input2 = nullptr;
  TF_Status* s = TF_NewStatus();
  
  // this should be of type 20 TF_RESOURCE
  TF_GetInput(ctx, 0, &input, s);
  if (input)
  {
    printf("bytes1 %d type %d dims %d\n", TF_TensorByteSize(input), TF_TensorType(input), TF_NumDims(input));
    dst = (char *)TF_TensorData(input);
    bytes = TF_TensorByteSize(input);
    printf("dst %p\n", dst);
  }

  // this is the value to set the resource 
  TF_GetInput(ctx, 1, &input2, s);
  if (input2)
  {
    printf("bytes2 %d type %d dims %d\n", TF_TensorByteSize(input2), TF_TensorType(input2), TF_NumDims(input2));
    src = (char *)TF_TensorData(input2);
    bytes = TF_TensorByteSize(input2);
    dims = TF_NumDims(input2);
    for (i = 0; i < dims; i++)
      printf("dim %d size %d\n", i, TF_Dim(input2, i));
    float* data = reinterpret_cast<float*>(src);
    printf("buf2 %p\n", data);
    
    for (i = 0; i < bytes/4; i++)
      printf("%f\n", data[i]);
      
    // copy from value to resource
//    for (i = 0; i < bytes; i++)
//      dst[i] = src[i];
  }    
}

static void AssVarOp_Delete(void *k) {
  printf("AssVarOp delete %p\n", k);
  if (k)
    delete static_cast<AssVarOp *>(k);
}

static void *RdVarOp_Create(TF_OpKernelConstruction *ctx) {
  printf("RdVarOp create\n");
  auto* kernel = new AssVarOp;

  TF_Status* s = TF_NewStatus();
  TF_OpKernelConstruction_GetAttrType(ctx, "T", &kernel->input_data_type, s);
  printf("get code1 is %d\n", TF_GetCode(s));

//  if (TF_GetCode(s) == TF_OK) {
    TF_OpKernelConstruction_GetAttrType(ctx, "type", &kernel->output_data_type,
                                        s);
//  }
  printf("get code2 is %d\n", TF_GetCode(s));

  
  TF_StringView name = TF_OpKernelConstruction_GetName(ctx);
  printf("name is %s\n", name.data);

  TF_DeleteStatus(s);
  return kernel;
}

static void RdVarOp_Compute(void *k, TF_OpKernelContext *ctx) {
  printf("RdVarOp compute\n");
  int num_inputs;
  int num_outputs;
  num_inputs = TF_NumInputs(ctx);
  num_outputs = TF_NumOutputs(ctx);
  printf("in %d out %d\n", num_inputs, num_outputs);
  
  TF_Tensor *input = nullptr, *input2 = nullptr;
  TF_Status* s = TF_NewStatus();
  TF_GetInput(ctx, 0, &input, s);
  if (input)
    printf("bytes1 %d type %d dims %d\n", TF_TensorByteSize(input), TF_TensorType(input), TF_NumDims(input));
  TF_GetInput(ctx, 1, &input2, s);
  if (input2)
    printf("bytes2 %d type %d dims %d\n", TF_TensorByteSize(input2), TF_TensorType(input2), TF_NumDims(input2));
}

static void RdVarOp_Delete(void *k) {
  printf("RdVarOp delete %p\n", k);
  if (k)
    delete static_cast<AssVarOp *>(k);
}

void SE_InitPlugin(SE_PlatformRegistrationParams* const params,
                   TF_Status* const status) {
  
  SP_Platform *platform;
  SP_PlatformFns *platform_fns;
  
  LOG(INFO) << "Salience plugin initialisation";

  platform = params->platform;
  platform_fns = params->platform_fns;
  
  *platform = {SP_PLATFORM_STRUCT_SIZE};
  
  // initialise the destroy funcs
  params->destroy_platform = DestroyPlatform;
  params->destroy_platform_fns = DestroyPlatformFns;
  
  // fill in platform params
  platform->type = "PPU";	// [A..Z]
  platform->name = "SALIENCE_REVA"; // [A..Z,_]
  platform->supports_unified_memory = false;
  platform->use_bfc_allocator = false;
  
  // populate platform functions
  platform_fns->get_device_count = GetDeviceCount;
  platform_fns->create_device = CreateDevice;
  platform_fns->destroy_device = DestroyDevice;
  platform_fns->create_device_fns = CreateDeviceFns;
  platform_fns->destroy_device_fns = DestroyDeviceFns;
  platform_fns->create_stream_executor = CreateStreamExecutor;
  platform_fns->destroy_stream_executor = DestroyStreamExecutor;
  platform_fns->create_timer_fns = CreateTimerFns;
  platform_fns->destroy_timer_fns = DestroyTimerFns;  

}

void TF_InitKernel() {
  LOG(INFO) << "Salience TF Kernel initialisation";
  
    
  const char* const DEVICE_PPU = "PPU";
  
  TF_Status* status = TF_NewStatus();
  TF_KernelBuilder* builder;
  
  builder = TF_NewKernelBuilder("AssignVariableOp", DEVICE_PPU,
                                 &AssVarOp_Create, &AssVarOp_Compute, &AssVarOp_Delete);
  TF_RegisterKernelBuilder("AssignVariableOp", builder, status);
  if (TF_GetCode(status) != TF_OK) {
   /* handle errors */ 
   printf("assignop failed\n");
  }
#ifdef nope
  builder = TF_NewKernelBuilder("ReadVariableOp", DEVICE_PPU,
                                 &RdVarOp_Create, &RdVarOp_Compute, &RdVarOp_Delete);
  TF_RegisterKernelBuilder("ReadVariableOp", builder, status);
  if (TF_GetCode(status) != TF_OK) {
   /* handle errors */ 
   printf("readop failed\n");
  }
#endif
  TF_DeleteStatus(status);
  
  
#ifdef nope
  TF_OpDefinitionBuilder* builder = TF_NewOpDefinitionBuilder("AssignVariableOp");
//  TF_OpDefinitionBuilderAddInput(builder, "input1: int32");
//  TF_OpDefinitionBuilderAddOutput(builder, "output1: int64");
//  TF_OpDefinitionBuilderAddAttr(builder, "attr: int32");
  TF_Status* status = TF_NewStatus();
  TF_RegisterOpDefinition(builder, status);
  if (TF_GetCode(status) != TF_OK) {
    // handle error
  }
#endif
}

} // extern "C"
