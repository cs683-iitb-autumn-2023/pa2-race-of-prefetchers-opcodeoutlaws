#include <algorithm>
#include <array>
#include <map>

#include "cache.h"

constexpr int PREFETCH_DEGREE = 3;
constexpr int PREFETCH_DISTANCE = 4;
const int INITIAL = 0;
const int ALLOCATED = 1;
const int TRAINING = 2;

struct tracker_entry {

  uint64_t ip = 0;              // the IP we're tracking
  int state = 0;                // state: Invalid = 0, Allocated = 1, Training = 2
  uint64_t last_cl_addr = 0;    // the last address accessed by this IP
  int direction = 0;            // direction: positive = 1, negative = -1;
  uint64_t last_used_cycle = 0; // use LRU to evict old IP trackers
};
struct lookahead_entry {
  uint64_t address = 0;
  int degree = 0; // degree remaining
  int direction = 1;
};

constexpr std::size_t TRACKER_SETS = 64;

std::map<CACHE*, lookahead_entry> lookahead;
std::map<CACHE*, std::array<tracker_entry, TRACKER_SETS>> trackers;

void CACHE::prefetcher_initialize() {}

uint32_t CACHE::prefetcher_cache_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type, uint32_t metadata_in)
{
  uint64_t cl_addr = addr >> LOG2_BLOCK_SIZE;
  // printf("%u\n", cl_addr);
  if (!cache_hit) {

    int state = ALLOCATED;
    int direction = 0;

    // get boundaries of tracking set
    auto set_begin = std::begin(trackers[this]);
    auto set_end = std::next(set_begin, TRACKER_SETS);

    // find the current ip within the set
    auto found = std::find_if(set_begin, set_end, [ip](tracker_entry x) { return x.ip == ip; });

    if (found != set_end) // entry exists
    {
      if (cl_addr > found->last_cl_addr) // computing direction
      {
        direction = 1;
      } else {
        direction = -1;
      }

      if (found->state == ALLOCATED) {
        state = TRAINING;
      } else if (found->state == TRAINING) {
        state = TRAINING;
        if (found->direction == direction) {
          // here we have estimated the direction correctly, hence we can issue prefetch request
          lookahead[this] = {(cl_addr + PREFETCH_DISTANCE) << LOG2_BLOCK_SIZE, PREFETCH_DEGREE, direction};
        }
      }
    } else { // entry does not exists
      found = std::min_element(set_begin, set_end, [](tracker_entry x, tracker_entry y) { return x.last_used_cycle < y.last_used_cycle; });
    }

    *found = {ip, state, cl_addr, direction, current_cycle};
  }
  return metadata_in;
}

uint32_t CACHE::prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr, uint32_t metadata_in)
{
  return metadata_in;
}

void CACHE::prefetcher_cycle_operate()
{
  // If a lookahead is active
  if (auto [old_pf_address, degree, direction] = lookahead[this]; degree > 0) {
    auto pf_address = old_pf_address + direction * (1 << LOG2_BLOCK_SIZE);

    // If the next step would exceed the degree or run off the page, stop
    if (virtual_prefetch || (pf_address >> LOG2_PAGE_SIZE) == (old_pf_address >> LOG2_PAGE_SIZE)) {
      // check the MSHR occupancy to decide if we're going to prefetch to this
      // level or not
      bool success = prefetch_line(0, 0, pf_address, (get_occupancy(0, pf_address) < get_size(0, pf_address) / 2), 0);
      if (success) {
        lookahead[this] = {pf_address, degree - 1, direction};
        // printf("prefetched %u old pf adddress = %u\n", pf_address >> LOG2_BLOCK_SIZE, old_pf_address >> LOG2_BLOCK_SIZE);
      }
      // If we fail, try again next cycle
    } else {
      lookahead[this] = {};
    }
  }
}

void CACHE::prefetcher_final_stats() {}
