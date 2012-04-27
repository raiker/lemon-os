#include <kmalloc.h>

class small_alloc {
public:
	small_alloc * next;
	size_t block_size;
	uint32_t num_blocks;
	void * blocks_start;
	uint8_t bitmask[]; //let's see if C++ supports flexible arrays...
}

struct large_alloc {
	size_t num_pages;
	int32_t magic;
}

#define PAGE_BITS 12
#define PAGE_SIZE (1 << PAGE_BITS) //4KiB

#define PAGE_SPLIT 2 //power of two less than page size that will do a large allocation

#define LARGE_PAGE_MAGIC 0x7fca82b9
#define SMALL_PAGE_MAGIC 0x298c98a4

static uint8_t memspace[0x1000000]; //16MiB of space
static uint8_t mem_bitmap[sizeof(memspace) >> PAGE_BITS >> 3]; //1 bit per 4KiB page

static small_alloc *small_allocations[PAGE_BITS - PAGE_SPLIT + 1];

//mallocs > 4KiB are allocated in units of 4KiB

void * memset(void * mem, uint8_t value, size_t size){
	for (uint8_t * ptr = mem; size > 0; size -= sizeof(uint8_t)){
		*ptr++ = value;
	}
	return mem;
}

void kmeminit(){
	memset(&mem_bitmap, sizeof(mem_bitmap), 0);
	memset(&small_allocations, sizeof(small_allocations), 0);
}

void * kmalloc(size_t size){
	if (size > PAGE_SIZE / 4){
		//large allocation
		size_t num_pages = (size + sizeof(large_alloc) + PAGE_SIZE - 1) >> PAGE_BITS;
		
		//search for a point in the map with enough contiguous pages free
		int32_t bitmap_loc = search_bitmap(num_pages);

		if (bitmap_loc == -1){
			return NULL;
		}

		//otherwise we're good, write to the bitmap to indicate that the pages are now reserved
		set_bitmask(bitmap_loc, num_pages);

		//write a large_alloc descriptor
		large_alloc *descriptor = (large_alloc*)(&memspace[bitmap_loc << PAGE_BITS]);
		descriptor->num_pages = num_pages;
		descriptor->magic = LARGE_PAGE_MAGIC;
		return (void*)(descriptor + 1);
	} else {
		//small allocation
	}
}

void kfree(void * alloc){
	//read the magic number
	int32_t magic = *(((int32_t*)alloc) - 1);

	if (magic == LARGE_PAGE_MAGIC){
		large_alloc *descriptor = ((large_alloc*)alloc) - 1;
		//translate the address to a page #
		int32_t page_num = ((uintptr_t)descriptor - (uintptr_t)memspace) >> PAGE_BITS;
		clear_bitmask(page_num, descriptor->num_pages);
		descriptor->magic = 0x00000000;
	}
}

#define BIT_TEST(x,n) ((x) & (1 << n))

//returns the page# of the first match found, or -1 on error
int32_t search_bitmap(size_t numpages){
	size_t pagesleft = numpages;
	for (int32_t i = 0; i < sizeof(memspace) && pagesleft != 0; i++){
		if (BIT_TEST(mem_bitmap[i >> 3], i & 0x7)){
			pagesleft--;
		} else {
			pagesleft = numpages;
		}
	}

	if (pagesleft == 0){
		//found it
		return i - numpages;
	} else {
		return -1;
	}
}

void set_bitmask(int32_t block_start, size_t numpages){
	uint8_t *ptr = mem_bitmap[block_start >> 3];
	uint8_t init_mask = 0xff >> (block_start & 0x7);
	*ptr++ |= init_mask;

	numpages -= (block_start & 0x7);

	while (numpages >= 8){
		*ptr++ = 0xff;
	}

	*ptr |= 0xff << (8 - num_pages);
}

//assumes that the bits are already set - bad stuff if they aren't
void clear_bitmask(int32_t block_start, size_t numpages){
	uint8_t *ptr = mem_bitmap[block_start >> 3];
	uint8_t init_mask = 0xff >> (block_start & 0x7);
	*ptr++ ^= init_mask;

	numpages -= (block_start & 0x7);

	while (numpages >= 8){
		*ptr++ ^= 0xff;
	}

	*ptr ^= 0xff << (8 - num_pages);
}
