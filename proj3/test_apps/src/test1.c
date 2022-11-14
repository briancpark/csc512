#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

__attribute__((noinline)) int8_t  sig_add_8(int8_t a, int8_t b)
{
  int8_t result = a;
  __asm__ __volatile__("add  %%bl,%%al"
                             :"=a"(result)
                             :"a"(result), "b"(b)
                             );

  return result;
}

__attribute__((noinline)) int8_t  sig_sub_8(int8_t a, int8_t b)
{
  int8_t result = a;
  __asm__ __volatile__("sub  %%bl,%%al"
                             :"=a"(result)
                             :"a"(result), "b"(b)
                             );

  return result;
}

__attribute__((noinline)) int8_t  sig_shift_8(int8_t a, int8_t n)
{
  int8_t result = a;
  __asm__ __volatile__("shl  %%cl,%%bl"
                             :"=b"(result)
                             :"c"(n), "b"(result)
                             );

  return result;
}

__attribute__((noinline)) void case8() {
  int8_t x = 1<<6;
  int8_t y = 1<<6;
  int8_t z = sig_shift_8(-1, 7);
  z = sig_shift_8(1, 7);
  int8_t m = -x;
  int8_t n = y+1;
  int8_t t1 = sig_add_8(x, y);
  int8_t t2 = sig_add_8(-x, -y-1);
  int8_t t3 = sig_sub_8(m, n);
  printf("case8: %d, %d, %d, %d, %d, %d, %d, %d\n", x, y, z, m, n, t1, t2, t3);
}

__attribute__((noinline)) int16_t  sig_add_16(int16_t a, int16_t b)
{
  int16_t result = a;
  __asm__ __volatile__("add  %%bx,%%ax"
                             :"=a"(result)
                             :"a"(result), "b"(b)
                             );

  return result;
}

__attribute__((noinline)) int16_t  sig_sub_16(int16_t a, int16_t b)
{
  int16_t result = a;
  __asm__ __volatile__("sub  %%bx,%%ax"
                             :"=a"(result)
                             :"a"(result), "b"(b)
                             );

  return result;
}

__attribute__((noinline)) int16_t  sig_shift_16(int16_t a, int16_t n)
{
  int16_t result = a;
  __asm__ __volatile__("shl  %%cl,%%bx"
                             :"=b"(result)
                             :"c"(n), "b"(result)
                             );

  return result;
}

__attribute__((noinline)) void case16() {
  int16_t x = 1<<14;
  int16_t y = 1<<14;
  int16_t z = sig_shift_16(-1, 15);
  z = sig_shift_16(1, 15);
  int16_t m = -x;
  int16_t n = y+1;
  int16_t t1 = sig_add_16(x, y);
  int16_t t2 = sig_add_16(-x, -y-1);
  int16_t t3 = sig_sub_16(m, n);
  printf("case16: %d, %d, %d, %d, %d, %d, %d, %d\n", x, y, z, m, n, t1, t2, t3);
}


__attribute__((noinline)) int32_t  sig_add_32(int32_t a, int32_t b)
{
  return a + b;
}

__attribute__((noinline)) int32_t  sig_sub_32(int32_t a, int32_t b)
{
  return a - b;
}

__attribute__((noinline)) int32_t  sig_shift_32(int32_t a, int32_t n)
{
  return a << n;
}

__attribute__((noinline)) void case32() {
  int32_t x = 1<<30;
  int32_t y = 1<<30;
  int32_t z = sig_shift_32(-1, 31);
  z = sig_shift_32(1, 31);
  int32_t m = -x;
  int32_t n = y+1;
  int32_t t1 = sig_add_32(x, y);
  int32_t t2 = sig_add_32(-x, -y-1);
  int32_t t3 = sig_sub_32(m, n);
  printf("case32: %d, %d, %d, %d, %d, %d, %d, %d\n", x, y, z, m, n, t1, t2, t3);
}

__attribute__((noinline)) int64_t  sig_add_64(int64_t a, int64_t b)
{
  return a + b;
}

__attribute__((noinline)) int64_t  sig_sub_64(int64_t a, int64_t b)
{
  return a - b;
}

__attribute__((noinline)) int64_t  sig_shift_64(int64_t a, int64_t n)
{
  return a << n;
}

__attribute__((noinline)) void case64() {
  int64_t x = 1L<<62;
  int64_t y = 1L<<62;
  int64_t z = sig_shift_64(-1, 63);
  z = sig_shift_64(1, 63);
  int64_t m = -x;
  int64_t n = y+1;
  int64_t t1 = sig_add_64(x, y);
  int64_t t2 = sig_add_64(-x, -y-1);
  int64_t t3 = sig_sub_64(m, n);
  printf("case64: %"PRId64", %"PRId64", %"PRId64", %"PRId64", %"PRId64", %"PRId64", %"PRId64", %"PRId64"\n", x, y, z, m, n, t1, t2, t3);
}

int main()
{
  case8();
  fflush(stdout);
  case16();
  fflush(stdout);
  case32();
  fflush(stdout);
  case64();
  fflush(stdout);
  return 0;
}