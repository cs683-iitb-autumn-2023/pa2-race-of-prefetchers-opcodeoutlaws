#include <algorithm>
#include <array>
#include <map>

#include "cache.h"

constexpr int PREFETCH_DEGREE = 2;
constexpr int PREFETCH_DISTANCE = 16;
const int INITIAL_DISTANCE = 12;

constexpr std::size_t TRACKER_SETS = 64;

const int INITIAL = 0;
const int ALLOCATED = 1;
const int TRAINING = 2;
const int MONITERING = 3;

struct tracker_entry {
  uint64_t ip = 0; // the IP we're tracking
  uint64_t last_cl_addr = 0;
  uint64_t A;
  uint64_t P;                   // the last address accessed by this IP
  short int direction = 0;      // the stride between the last two addresses accessed by this IP
  uint64_t last_used_cycle = 0; // use LRU to evict old IP trackers
  short int state = INITIAL;
};

struct lookahead_entry {
  uint64_t address = 0;
  short int direction = 1; // 1 means positive and 0 means negative
  int degree = 0;          // degree remaining
};

std::map<CACHE*, lookahead_entry> lookahead;
std::map<CACHE*, std::array<tracker_entry, TRACKER_SETS>> trackers;

void CACHE::prefetcher_initialize() { std::cout << "Stream Prefetcher with degree = " << PREFETCH_DEGREE << " and distance = " << PREFETCH_DISTANCE << endl; }

void CACHE::prefetcher_cycle_operate()
{
  // If a lookahead is active
  if (auto [old_pf_address, direction, degree] = lookahead[this]; degree > 0) {
    uint64_t pf_address;
    pf_address = old_pf_address + ((direction ? 1 : -1) * (1 << LOG2_BLOCK_SIZE));

    // If the next step would exceed the degree or run off the page, stop
    if (virtual_prefetch || (pf_address >> LOG2_PAGE_SIZE) == (old_pf_address >> LOG2_PAGE_SIZE)) {
      // check the MSHR occupancy to decide if we're going to prefetch to this
      // level or not
      bool success = prefetch_line(0, 0, pf_address, (get_occupancy(0, pf_address) < get_size(0, pf_address) / 2), 0);
      if (success) {
        lookahead[this] = {pf_address, direction, degree - 1};
        // printf("prefetched %u old pf adddress = %u and direction = %d\n", pf_address >> LOG2_BLOCK_SIZE, old_pf_address >> LOG2_BLOCK_SIZE, direction);
        // if (direction == 0) printf("**********************************************\n");
      }
      // If we fail, try again next cycle
    } else {
      lookahead[this] = {};
    }
  }
}

uint32_t CACHE::prefetcher_cache_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type, uint32_t metadata_in)
{

  uint64_t cl_addr = addr >> LOG2_BLOCK_SIZE;
  // printf("%u\n", cl_addr);
  if (!cache_hit) {

    short int state;
    short int direction = 0;

    // get boundaries of tracking set
    auto set_begin = std::begin(trackers[this]);
    auto set_end = std::next(set_begin, TRACKER_SETS);
    uint64_t new_A = cl_addr;
    uint64_t new_P = cl_addr;

    // find the current ip within the set
    auto found = std::find_if(set_begin, set_end, [ip](tracker_entry x) { return x.ip == ip; });

    if (found != set_end) // entry exists
    {
      state = found->state;
      // printf("%u -------------- %u\n", cl_addr, found->last_cl_addr);
      if (cl_addr > found->last_cl_addr) // computing direction
      {
        direction = 1;
      } else {
        direction = 0;
      }

      if (found->state == ALLOCATED) {
        state = TRAINING;
      } else if (found->state == TRAINING) {
        if (found->direction == direction) {
          state = MONITERING;
          new_P = cl_addr + INITIAL_DISTANCE;
        } else {
          state = ALLOCATED;
          new_A = cl_addr;
        }
      } else if (state == MONITERING) {
        if ((cl_addr >= found->A && cl_addr <= found->P) || (cl_addr <= found->A && cl_addr >= found->P)) {
          
          // send the prefetch request
          lookahead[this] = {found->P << LOG2_BLOCK_SIZE, found->direction, PREFETCH_DEGREE};
          
        
          if (std::abs((long) found->P - (long) found->A) == PREFETCH_DISTANCE) {
            new_A = found->A + (direction ? 1 : -1) * PREFETCH_DEGREE;
            new_P = found->P + (direction ? 1 : -1) * PREFETCH_DEGREE;
          } else {
            if (found->direction == 1)
              new_P = std::min(found->A + PREFETCH_DISTANCE, found->P + PREFETCH_DEGREE);
            else
              new_P = std::max(found->A - PREFETCH_DISTANCE, found->P - PREFETCH_DEGREE);
          }
        } else {
          state = ALLOCATED;
          new_A = cl_addr;
        }
      }

    } else { // entry does not exists
      found = std::min_element(set_begin, set_end, [](tracker_entry x, tracker_entry y) { return x.last_used_cycle < y.last_used_cycle; });
      state = ALLOCATED;
      new_A = cl_addr;
    }

    *found = {ip, cl_addr, new_A, new_P, direction, current_cycle, state};
  }
  return metadata_in;
}

uint32_t CACHE::prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr, uint32_t metadata_in)
{
  return metadata_in;
}

void CACHE::prefetcher_final_stats() {}