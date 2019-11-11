
#include <gba_console.h>
#include <stdio.h>

#define LOAD_STORE_IMM(L, B, rD, rB, offset) ((3<<13) | (B<<12) | (L<<11) | (offset<<6) | (rB<<3) | rD)
#define LOAD_STORE_HALF_IMM(L, rD, rB, offset) ((1<<15) | (L<<11) | (offset<<6) | (offset<<6) | (rB<<3) | rD)

#define LDRB_RD_RB_IMM(rD, rB, offset) LOAD_STORE_IMM(1, 1, rD, rB, offset)
#define STRB_RD_RB_IMM(rD, rB, offset) LOAD_STORE_IMM(0, 1, rD, rB, offset)
#define LDRH_RD_RB_IMM(rD, rB, offset) LOAD_STORE_HALF_IMM(1, rD, rB, offset)
#define STRH_RD_RB_IMM(rD, rB, offset) LOAD_STORE_HALF_IMM(0, rD, rB, offset)

struct Test {
  const char* title;
  u16 patch;
  u32 location;
  u32 expected;
  u32 user;
};

struct Battery {
  const char* title;
  struct Test* tests;
  int count;
};

static struct Test ldrb_tests[] = {
  /* LRDB ALIGNED */
  {
    .title = "LDRB #0 A",
    .patch = LDRB_RD_RB_IMM(0, 0, 0),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0
  },
  {
    .title = "LDRB #1 A",
    .patch = LDRB_RD_RB_IMM(0, 0, 1),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0
  },
  {
    .title = "LDRB #2 A",
    .patch = LDRB_RD_RB_IMM(0, 0, 2),
    .location = 0x03004000,
    .expected = 0x4733FEED,
    .user = 0
  },
  {
    .title = "LDRB #3 A",
    .patch = LDRB_RD_RB_IMM(0, 0, 3),
    .location = 0x03004000,
    .expected = 0x4470FEED,
    .user = 0
  },
  {
    .title = "LDRB #4 A",
    .patch = LDRB_RD_RB_IMM(0, 0, 4),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0
  },
  {
    .title = "LDRB #5 A",
    .patch = LDRB_RD_RB_IMM(0, 0, 5),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0
  },
  {
    .title = "LDRB #6 A",
    .patch = LDRB_RD_RB_IMM(0, 0, 6),
    .location = 0x03004000,
    .expected = 0x4777FEED,
    .user = 0
  },
  {
    .title = "LDRB #7 A",
    .patch = LDRB_RD_RB_IMM(0, 0, 7),
    .location = 0x03004000,
    .expected = 0x8870FEED,
    .user = 0
  },
  
  /* LDRB UNALIGNED */
  {
    .title = "LDRB #0 U",
    .patch = LDRB_RD_RB_IMM(0, 0, 0),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0
  },
  {
    .title = "LDRB #1 U",
    .patch = LDRB_RD_RB_IMM(0, 0, 1),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0
  },
  {
    .title = "LDRB #2 U",
    .patch = LDRB_RD_RB_IMM(0, 0, 2),
    .location = 0x03004102,
    .expected = 0xFEED4733,
    .user = 0
  },
  {
    .title = "LDRB #3 U",
    .patch = LDRB_RD_RB_IMM(0, 0, 3),
    .location = 0x03004102,
    .expected = 0xFEED4470,
    .user = 0
  },
  {
    .title = "LDRB #4 U",
    .patch = LDRB_RD_RB_IMM(0, 0, 4),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0
  },
  {
    .title = "LDRB #5 U",
    .patch = LDRB_RD_RB_IMM(0, 0, 5),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0
  },
  {
    .title = "LDRB #6 U",
    .patch = LDRB_RD_RB_IMM(0, 0, 6),
    .location = 0x03004102,
    .expected = 0xFEED4777,
    .user = 0
  },
  {
    .title = "LDRB #7 U",
    .patch = LDRB_RD_RB_IMM(0, 0, 7),
    .location = 0x03004102,
    .expected = 0xFEED8870,
    .user = 0
  }
};

static struct Test strb_tests[] = {
  /* STRB ALIGNED */
  {
    .title = "STRB #0 A",
    .patch = STRB_RD_RB_IMM(3, 0, 0),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0x11
  },
  {
    .title = "STRB #1 A",
    .patch = STRB_RD_RB_IMM(3, 0, 1),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0x22
  },
  {
    .title = "STRB #2 A",
    .patch = STRB_RD_RB_IMM(3, 0, 2),
    .location = 0x03004000,
    .expected = 0x4733FEED,
    .user = 0x33
  },
  {
    .title = "STRB #3 A",
    .patch = STRB_RD_RB_IMM(3, 0, 3),
    .location = 0x03004000,
    .expected = 0x4470FEED,
    .user = 0x44
  },
  {
    .title = "STRB #4 A",
    .patch = STRB_RD_RB_IMM(3, 0, 4),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0x55
  },
  {
    .title = "STRB #5 A",
    .patch = STRB_RD_RB_IMM(3, 0, 5),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0x66
  },
  {
    .title = "STRB #6 A",
    .patch = STRB_RD_RB_IMM(3, 0, 6),
    .location = 0x03004000,
    .expected = 0x4777FEED,
    .user = 0x77
  },
  {
    .title = "STRB #7 A",
    .patch = STRB_RD_RB_IMM(3, 0, 7),
    .location = 0x03004000,
    .expected = 0x8870FEED,
    .user = 0x88
  },
  
  /* STRB UNALIGNED */
  {
    .title = "STRB #0 U",
    .patch = STRB_RD_RB_IMM(3, 0, 0),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0x11
  },
  {
    .title = "STRB #1 U",
    .patch = STRB_RD_RB_IMM(3, 0, 1),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0x22
  },
  {
    .title = "STRB #2 U",
    .patch = STRB_RD_RB_IMM(3, 0, 2),
    .location = 0x03004102,
    .expected = 0xFEED4733,
    .user = 0x33
  },
  {
    .title = "STRB #3 U",
    .patch = STRB_RD_RB_IMM(3, 0, 3),
    .location = 0x03004102,
    .expected = 0xFEED4470,
    .user = 0x44
  },
  {
    .title = "STRB #4 U",
    .patch = STRB_RD_RB_IMM(3, 0, 4),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0x55
  },
  {
    .title = "STRB #5 U",
    .patch = STRB_RD_RB_IMM(3, 0, 5),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0x66
  },
  {
    .title = "STRB #6 U",
    .patch = STRB_RD_RB_IMM(3, 0, 6),
    .location = 0x03004102,
    .expected = 0xFEED4777,
    .user = 0x77
  },
  {
    .title = "STRB #7 U",
    .patch = STRB_RD_RB_IMM(3, 0, 7),
    .location = 0x03004102,
    .expected = 0xFEED8870,
    .user = 0x88
  }
};

static struct Test ldrh_tests[] = {
  /* LDRH ALIGNED */
  {
    .title = "LDRH #0 A",
    .patch = LDRH_RD_RB_IMM(0, 0, 0),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0
  },
  {
    .title = "LDRH #1 A",
    .patch = LDRH_RD_RB_IMM(0, 0, 1),
    .location = 0x03004000,
    .expected = 0x4433FEED,
    .user = 0
  },
  {
    .title = "LDRH #2 A",
    .patch = LDRH_RD_RB_IMM(0, 0, 2),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0
  },
  {
    .title = "LDRH #3 A",
    .patch = LDRH_RD_RB_IMM(0, 0, 3),
    .location = 0x03004000,
    .expected = 0x8877FEED,
    .user = 0
  },
  {
    .title = "LDRH #4 A",
    .patch = LDRH_RD_RB_IMM(0, 0, 4),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0
  },
  {
    .title = "LDRH #5 A",
    .patch = LDRH_RD_RB_IMM(0, 0, 5),
    .location = 0x03004000,
    .expected = 0xCCDDFEED,
    .user = 0
  },
  {
    .title = "LDRH #6 A",
    .patch = LDRH_RD_RB_IMM(0, 0, 6),
    .location = 0x03004000,
    .expected = 0x4770FEED,
    .user = 0
  },
  {
    .title = "LDRH #7 A",
    .patch = LDRH_RD_RB_IMM(0, 0, 7),
    .location = 0x03004000,
    .expected = 0x0000FEED,
    .user = 0
  },
  
  /* LDRH UNALIGNED */
  {
    .title = "LDRH #0 U",
    .patch = LDRH_RD_RB_IMM(0, 0, 0),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0
  },
  {
    .title = "LDRH #1 U",
    .patch = LDRH_RD_RB_IMM(0, 0, 1),
    .location = 0x03004102,
    .expected = 0xFEED4433,
    .user = 0
  },
  {
    .title = "LDRH #2 U",
    .patch = LDRH_RD_RB_IMM(0, 0, 2),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0
  },
  {
    .title = "LDRH #3 U",
    .patch = LDRH_RD_RB_IMM(0, 0, 3),
    .location = 0x03004102,
    .expected = 0xFEED8877,
    .user = 0
  },
  {
    .title = "LDRH #4 U",
    .patch = LDRH_RD_RB_IMM(0, 0, 4),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0
  },
  {
    .title = "LDRH #5 U",
    .patch = LDRH_RD_RB_IMM(0, 0, 5),
    .location = 0x03004102,
    .expected = 0xFEEDCCDD,
    .user = 0
  },
  {
    .title = "LDRH #6 U",
    .patch = LDRH_RD_RB_IMM(0, 0, 6),
    .location = 0x03004102,
    .expected = 0xFEED4770,
    .user = 0
  },
  {
    .title = "LDRH #7 U",
    .patch = LDRH_RD_RB_IMM(0, 0, 7),
    .location = 0x03004102,
    .expected = 0xFEED0000,
    .user = 0
  }
};

static struct Battery batteries[] = {
  {
    .title = "IWRAM OPEN BUS - LDRB",
    .tests = &ldrb_tests[0],
    .count = sizeof(ldrb_tests)/sizeof(struct Test)
  },
  {
    .title = "IWRAM OPEN BUS - STRB",
    .tests = &strb_tests[0],
    .count = sizeof(strb_tests)/sizeof(struct Test)
  },
  {
    .title = "IWRAM OPEN BUS - LDRH",
    .tests = &ldrh_tests[0],
    .count = sizeof(ldrh_tests)/sizeof(struct Test)
  }
};

__attribute__((naked)) u32 run_test(u32 /*r0*/ location, u32 /*r1*/ user) {
  asm(
    "push {lr}\n"
    "mov r3, r1\n" // r3 = user arg
    "orr r2, r0, #1\n"
    "add r0, r0, #0x16\n" // offset to my_data = 0x16
    "ldr r1, =0xABAD1DEA\n" // a bad address
    "mov r14, r15\n"
    "bx r2\n"
    "pop {r15}\n"
  );
}

void keypress_wait(int key) {
  vu16* keypad = (vu16*)0x04000130;
  
  while (*keypad & key);
  while (~(*keypad) & key);
}

int main(void) {
  consoleDemoInit();

  /* Haha, please don't tell anyone. */
  vu16* pal = (vu16*)0x05000000;
  pal[0] = 0x7FFF;
  pal[0x1E2/2] = 0;
  
  /* Base payload will be copied to unaligned or aligned positions.
   * First instruction will be patched for varying test cases.
   */
  u8 payload[] = {
                // r0 = my_data, r1 = bad address
                // _start:       ALIGNED
    0x80, 0x78, // ldrb r0, [r0, #2] <- patched by individual tests
    0x08, 0x68, // ldr r0, [r1]  UNALIGNED
    0x70, 0x47, // bx lr         ALIGNED
                // padding:
    0xED, 0xFE, 0xED, 0xFE, 0xED, 0xFE, 0xED, 0xFE,
    0xED, 0xFE, 0xED, 0xFE, 0xED, 0xFE, 0xED, 0xFE, 
    0x11, 0x22, // my_data:      UNALIGNED
    0x33, 0x44, //               ALIGNED
    0x55, 0x66, //               UNALIGNED
    0x77, 0x88, //               ALIGNED
    0xBB, 0xAA, //               UNALIGNED
    0xDD, 0xCC, //               ALIGNED
    0xFF, 0xEE, //               UNALIGNED
  };
  
  int battery_count = sizeof(batteries)/sizeof(struct Battery);
  
  int total;
  int passed;
  
test:
  total = 0;
  passed = 0;
  
  for (int i = 0; i < battery_count; i++) {
    struct Battery* battery = &batteries[i];
    
    printf("\e[1;1H\e[2J%s (%d/%d)\n\n", battery->title, i + 1, battery_count);
    
    total += battery->count;
    
    for (int j = 0; j < battery->count; j++) {
      struct Test* test = &battery->tests[j];

      printf("%s ", test->title);

      u8* ram = (u8*)test->location; 

      /* Copy payload */
      for (int k = 0; k < sizeof(payload); k++) {
        ram[k] = payload[k];
      }

      /* Patch first instruction */
      ram[0] = test->patch & 0xFF;
      ram[1] = test->patch >> 8;

      u32 actual = run_test(test->location, test->user);

      if (test->expected == actual) {
        printf("%08lx OK\n", actual);
        passed++;
      } else {
        printf("%08lx ! %08lx\n", actual, test->expected);
      }
    }
    
    keypress_wait(1);
  }
  
  printf("\e[1;1H\e[2J");
  printf("passed: %d total: %d\n", passed, total);

  keypress_wait(1);
  goto test;
}
