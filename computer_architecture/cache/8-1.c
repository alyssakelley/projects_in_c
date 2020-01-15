// Author: Alyssa Kelley
// Assignment 8

// Note: Source code from Eric Wills, and I collaborated with 
// Anne Glickenhaus and Kristine Stecker in CIS 314 as we worked on
// this project together. 

#include <stdio.h>
#include <stdlib.h>

struct Line {
  unsigned char data[4];
  unsigned int tag;
  unsigned char valid;
};

struct Cache {
  struct Line *lines;
  int numLines;
};

unsigned int getOffset(unsigned int address) {
  // 4B blocks, so offset is bits 0-1
  return address & 0x3;
}

unsigned int getSet(unsigned int address) {
  // 16 sets, so offset is bits 2-6
  return (address >> 2) & 0xF;
}

unsigned int getTag(unsigned int address) {
  // Offset and set are 6 bits total, so tag is high-order 26 bits
  return address >> 6;
}

struct Cache* mallocCache(int numLines) {
  struct Cache* cache = (struct Cache *) malloc(sizeof(struct Cache)); // casting -> pointer
  cache -> numLines = numLines;
  struct Line* lines = (struct Line *) malloc(sizeof(struct Line));
  cache -> lines = lines;
  // to initialize cache so have everything set to 0 to begin with.
  for(int i = 0; i< numLines; i++)
  {
    cache -> lines[i].valid = 0;
    cache -> lines[i].tag = 0;
    for(int j=0; j<4; j++)
    {
      cache -> lines[i].data[j] = 0;
    }
  }
  return cache;
}

void freeCache(struct Cache *cache) {
  free(cache->lines);
  free(cache);
}

void printCache(struct Cache *cache) {
  // TODO - print all valid lines in the cache. 
  // This runs when you hit p.
  for(int i = 0; i<cache->numLines; i++) // going through the lines in the cache
  {
    if(cache->lines[i].valid == 1) // making sure it is valid
    {
      printf("set: %d - tag: %d - valid: %d - data:", i, cache->lines[i].tag, cache->lines[i].valid);
      for(int j=0; j<4; j++)
      {
      printf(" %.2x", cache->lines[i].data[j]); // printing the cache data as two lower case hex values if it is valid.
      }
    printf("\n");
  }
}
}

void readValue(struct Cache *cache, unsigned int address) {
  // parse the address
  unsigned int set = getSet(address);
  unsigned int tag = getTag(address); // tag is like a key so you need to make sure the tags (aka keys) match up for it to be a match -- think of this list hash tables
  unsigned int offset = getOffset(address);

  // Block we need
  printf("looking for set: %d - tag: %d\n", set, tag);
  struct Line* index = &cache->lines[set]; // Direct map so we go look in here and get the lines directly

  // Making sure it is valid
  if(index->valid == 0)
  {
    printf("no valid set found - miss! \n");
  }
  else if(index->tag == tag) // It is valid, and the tags match... it is a hit!
  {
    printf("found set: %d - tag: %d - valid: %u - data: ", set, index->tag, offset, index->valid); // this is the print block
    printf(" %.2x", index->data[offset]); // printing the offset with two lower case hex values.
    printf("\n");
    printf("hit!\n");
  }
  else // it is valid, but it does not match - it is a miss.
  {
    // print block
    printf("found set: %d - tag: %d offset: %u - valid: %d - data: ", set, index->tag, offset, index->valid);
    // print data
    printf(" %.2x", index->data[offset]);
    printf("\n");
    printf("tags don't match - miss!\n");
  }
}

void writeValue(struct Cache *cache, unsigned int address, unsigned char *newData) {
  // Calculate set and tag for address
  unsigned int s = getSet(address);
  unsigned int t = getTag(address);
  
  // Get pointer to cache line in the specified set
  struct Line *line = &cache->lines[s];

  // Determine if we have a valid line in the cache that does not contain the
  // specified address - we detect this by checking for a tag mismatch
  if (line->valid && line->tag != t) {
    unsigned char *data = line->data;
    printf("evicting line - set: %x - tag: %x - valid: %u - data: %.2x %.2x %.2x %.2x\n",
        s, line->tag, line->valid, data[0], data[1], data[2], data[3]);
  }
  
  // Copy new data to line (could use memcpy here instead)
  for (int i = 0; i < 4; ++i) {
    line->data[i] = newData[i];
  }

  // Update line tag, mark line as valid
  line->tag = t;
  line->valid = 1;

  printf("wrote set: %x - tag: %x - valid: %u - data: %.2x %.2x %.2x %.2x\n",
      s, line->tag, line->valid, newData[0], newData[1], newData[2], newData[3]);
}

int main() {  
  struct Cache *cache = mallocCache(16);

  // Loop until user enters 'q'
  char c;
  do {
    printf("Enter 'r' for read, 'w' for write, 'p' to print, 'q' to quit: ");
    scanf(" %c", &c);
    
    if (c == 'r') {     
      printf("Enter 32-bit unsigned hex address: ");
      
      unsigned int a;
      scanf(" %x", &a);
      
      readValue(cache, a);
    } else if (c == 'w') {
      printf("Enter 32-bit unsigned hex address: ");
      
      unsigned int a;
      scanf(" %x", &a);

      printf("Enter 32-bit unsigned hex value: ");
      
      unsigned int v;
      scanf(" %x", &v);
      
      // Get byte pointer to v
      unsigned char *data = (unsigned char *)&v;

      writeValue(cache, a, data);
    } else if (c == 'p') {
      printCache(cache);
    }
  } while (c != 'q');

  freeCache(cache);
}
