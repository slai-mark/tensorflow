/* Copyright 2022 Salience...
==============================================================================*/
#include "ppu_stream_exec.h"
#include <stdio.h>
#include <malloc.h>

extern "C" {

namespace salience {
namespace stream_exec {

#define SLAI_DEBUG printf

/*** Functions for creating SP_StreamExecutor ***/

void Allocate(const SP_Device* const device, uint64_t size,
              int64_t memory_space, SP_DeviceMemoryBase* const mem) {
  mem->struct_size = SP_DEVICE_MEMORY_BASE_STRUCT_SIZE;
  mem->opaque = malloc(size);
  mem->size = size;
  SLAI_DEBUG("Allocate %d bytes returning %p\n", size, mem->opaque);
  *(float *)(mem->opaque) = 12345678.0;
}

void Deallocate(const SP_Device* const device, SP_DeviceMemoryBase* const mem) {
  SLAI_DEBUG("Deallocate %p\n", mem->opaque);
  free(mem->opaque);
  mem->size = 0;
  mem->opaque = 0;
}

void* HostMemoryAllocate(const SP_Device* const device, uint64_t size) {
  SLAI_DEBUG("Host mem alloc\n");
  return nullptr;
}

void HostMemoryDeallocate(const SP_Device* const device, void* mem) {
  SLAI_DEBUG("Host mem dealloc\n");
}

TF_Bool GetAllocatorStats(const SP_Device* const device,
                          SP_AllocatorStats* const stats) {
  SLAI_DEBUG("get stats\n");
  if (stats) {
    stats->bytes_limit = 0x300000;
    return true;
  }
  return false;
}

TF_Bool DeviceMemoryUsage(const SP_Device* const device, int64_t* const free,
                          int64_t* const total) {
  SLAI_DEBUG("DeviceMemoryUsage\n");
  if (free)
    *free = 0x500000;
  if (total)
    *total = 0x600000;
  return true;
}

static int streamno = 1;
void CreateStream(const SP_Device* const device, SP_Stream* stream,
                  TF_Status* const status) {
  TF_SetStatus(status, TF_OK, "");
  *stream = new SP_Stream_st(streamno++);
  SLAI_DEBUG("create stream %d\n", (*stream)->stream_id);
}

void DestroyStream(const SP_Device* const device, SP_Stream stream) {
  SLAI_DEBUG("destroy stream %d\n", stream->stream_id);  
}

void CreateStreamDependency(const SP_Device* const device, SP_Stream dependent,
                            SP_Stream other, TF_Status* const status) {
  SLAI_DEBUG("create stream dep %d on %d\n", dependent->stream_id, other->stream_id);
  TF_SetStatus(status, TF_OK, "");  
}

void GetStreamStatus(const SP_Device* const device, SP_Stream stream,
                     TF_Status* const status) {
  SLAI_DEBUG("get stream status\n");  
}
                     
static int eventid = 1;
void CreateEvent(const SP_Device* const device, SP_Event* event,
                 TF_Status* const status) {
  TF_SetStatus(status, TF_OK, "");
  *event = new SP_Event_st(eventid++);
  SLAI_DEBUG("created event %d\n", (*event)->event_id);  
}

void DestroyEvent(const SP_Device* const device, SP_Event event) {
  SLAI_DEBUG("destroy event %d\n", event->event_id);  
}

SE_EventStatus GetEventStatus(const SP_Device* const device, SP_Event event) {
//
  return SE_EVENT_COMPLETE;
}

void RecordEvent(const SP_Device* const device, SP_Stream stream,
                 SP_Event event, TF_Status* const status) {
  SLAI_DEBUG("RecordEvent stream %d event %d\n", stream->stream_id, event->event_id);
  TF_SetStatus(status, TF_OK, "");
}

void WaitForEvent(const SP_Device* const device, SP_Stream stream,
                  SP_Event event, TF_Status* const status) {
  SLAI_DEBUG("WaitForEvent\n");
}

void CreateTimer(const SP_Device* const device, SP_Timer* timer,
                 TF_Status* const status) {
  SLAI_DEBUG("CreateTimer\n");
}

void DestroyTimer(const SP_Device* const device, SP_Timer timer) {
  SLAI_DEBUG("DestroyTimer\n");
}

void StartTimer(const SP_Device* const device, SP_Stream stream, SP_Timer timer,
                TF_Status* const status) {
  SLAI_DEBUG("StartTimer\n");
}

void StopTimer(const SP_Device* const device, SP_Stream stream, SP_Timer timer,
               TF_Status* const status) {
  SLAI_DEBUG("StopTimer\n");
}

void MemcpyDToH(const SP_Device* const device, SP_Stream stream, void* host_dst,
                const SP_DeviceMemoryBase* const device_src, uint64_t size,
                TF_Status* const status) {
  SLAI_DEBUG("MemcpyDToH stream %d dst %p/%d src %p/%d\n",
  	     stream->stream_id, host_dst, size, device_src->opaque, device_src->size);
  uint8_t *src, *dst;
  
  src = (uint8_t *)device_src->opaque;
  dst = (uint8_t *)host_dst;
  for (int i = 0; i < size; i++)
    dst[i] = src[i];
  
  TF_SetStatus(status, TF_OK, "");}

void MemcpyHToD(const SP_Device* const device, SP_Stream stream,
                SP_DeviceMemoryBase* const device_dst, const void* host_src,
                uint64_t size, TF_Status* const status) {
  SLAI_DEBUG("MemcpyHToD stream %d dst %p/%d host src %p/%d\n",
  	     stream->stream_id, device_dst->opaque, device_dst->size, host_src, size);
  uint8_t *src, *dst;
  
  src = (uint8_t *)host_src;
  dst = (uint8_t *)device_dst->opaque;
  for (int i = 0; i < size; i++)
    dst[i] = src[i];
  
  TF_SetStatus(status, TF_OK, "");
}

void SyncMemcpyDToH(const SP_Device* const device, void* host_dst,
                    const SP_DeviceMemoryBase* const device_src, uint64_t size,
                    TF_Status* const status) {
  SLAI_DEBUG("SyncMemcpyDToH\n");
}

void SyncMemcpyHToD(const SP_Device* const device,
                    SP_DeviceMemoryBase* const device_dst, const void* host_src,
                    uint64_t size, TF_Status* const status) {
  SLAI_DEBUG("SyncMemcpyHToD\n");
}

void BlockHostForEvent(const SP_Device* const device, SP_Event event,
                       TF_Status* const status) {
  SLAI_DEBUG("BlockHostForEvent\n");
}

void SynchronizeAllActivity(const SP_Device* const device,
                            TF_Status* const status) {
  SLAI_DEBUG("SynchronizeAllActivity\n");
}

TF_Bool HostCallback(const SP_Device* const device, SP_Stream stream,
                     SE_StatusCallbackFn const callback_fn,
                     void* const callback_arg) {
  SLAI_DEBUG("HostCallback\n");
  return true;
}

void MemZero(const SP_Device* device, SP_Stream stream,
             SP_DeviceMemoryBase* location, uint64_t size, TF_Status* status) {
  SLAI_DEBUG("MemZero\n");
}

void Memset(const SP_Device* device, SP_Stream stream,
            SP_DeviceMemoryBase* location, uint8_t pattern, uint64_t size,
            TF_Status* status) {
  SLAI_DEBUG("MemSet\n");
}

void Memset32(const SP_Device* device, SP_Stream stream,
              SP_DeviceMemoryBase* location, uint32_t pattern, uint64_t size,
              TF_Status* status) {
  SLAI_DEBUG("MemSet32\n");
}


void PopulateStreamExecutor(SP_StreamExecutor* se) {
  *se = {SP_STREAMEXECUTOR_STRUCT_SIZE};
  
  se->allocate = Allocate;
  se->deallocate = Deallocate;
  se->host_memory_allocate = HostMemoryAllocate;
  se->host_memory_deallocate = HostMemoryDeallocate;
  se->get_allocator_stats = GetAllocatorStats;
  se->device_memory_usage = DeviceMemoryUsage;
  se->create_stream = CreateStream;
  se->destroy_stream = DestroyStream;
  se->create_stream_dependency = CreateStreamDependency;
  se->get_stream_status = GetStreamStatus;
  se->create_event = CreateEvent;
  se->destroy_event = DestroyEvent;
  se->get_event_status = GetEventStatus;
  se->record_event = RecordEvent;
  se->wait_for_event = WaitForEvent;
  se->create_timer = CreateTimer;
  se->destroy_timer = DestroyTimer;
  se->start_timer = StartTimer;
  se->stop_timer = StopTimer;
  se->memcpy_dtoh = MemcpyDToH;
  se->memcpy_htod = MemcpyHToD; 
  se->sync_memcpy_dtoh = SyncMemcpyDToH; 
  se->sync_memcpy_htod = SyncMemcpyHToD; 
  se->block_host_for_event = BlockHostForEvent; 
  se->synchronize_all_activity = SynchronizeAllActivity; 
  se->host_callback = HostCallback; 
  se->mem_zero = MemZero; 
  se->memset = Memset; 
  se->memset32 = Memset32; 
} 

uint64_t Nanoseconds(SP_Timer timer) { return timer->timer_id; }

void PopulateTimerFns(SP_TimerFns* timer_fns) { 
  timer_fns->nanoseconds = Nanoseconds; 
}

} // namespace stream_exec
} // namespace salience

} // extern "C"
