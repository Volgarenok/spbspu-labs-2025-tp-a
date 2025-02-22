#include <sys/syscall.h>
#include <unistd.h>

int main()
{
  const char* output = "puzikov.herman\n";

  asm volatile(
      "mov $1, %%rax;"
      "mov $1, %%rdi;"
      "mov %0, %%rsi;"
      "mov $15, %%rdx;"
      "syscall;"
      :
      : "r"(output)
      : "%rax", "%rdi", "%rsi", "%rdx");

  return 0;
}
