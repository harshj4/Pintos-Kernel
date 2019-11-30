#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  void * esp = f->esp;
  printf ("system call!\n");
  // printf ("%x %x\n", esp, (void (*) (void)) esp);
  printf ("%x : %x %x\n", esp, ((char *)f->esp), *((char *)f->esp));

  switch (*((char *)f->esp))
  {
  case SYS_HALT: // args: 0
    printf("SYS_HALT\n");
    break;
  case SYS_EXIT: // args: 1
    printf("SYS_EXIT\n");
    thread_exit();
    break;
  case SYS_EXEC: // args: 1
    printf("SYS_EXEC\n");
    break;
  case SYS_WAIT: // args: 1
    printf("SYS_WAIT\n");
    break;
  case SYS_CREATE: // args: 2
    printf("SYS_CREATE\n");
    break;
  case SYS_REMOVE: // args: 1
    printf("SYS_REMOVE\n");
    break;
  case SYS_OPEN: // args: 1
    printf("SYS_OPEN\n");
    break;
  case SYS_FILESIZE: // args: 1
    printf("SYS_FILESIZE\n");
    break;
  case SYS_READ: // args: 3
    printf("SYS_READ\n");
    break;
  case SYS_WRITE: // args: 3
    printf("SYS_WRITE\n");
    printf("ESP is %x\n",esp);
    int32_t * arg0 = esp + 4;
    char ** arg1 = esp + 8;
    int32_t * arg2 = esp + 12;
    printf("SYS_WRITE:%x\n", *arg0);
    printf("size:%d || %x\n", *arg2, *arg2);
    printf("%s\n", (char *)(*arg1));
    // printf("BUFFER:");
    // for(int i=0;i<*arg2;i++)
    //   printf("%c %x\n", arg1[i], arg1[i]);
    // printf("\nEND\n");
      
    break;
  case SYS_SEEK: // args: 2
    printf("SYS_SEEK\n");
    break;
  case SYS_TELL: // args: 1
    printf("SYS_TELL\n");
    break;
  case SYS_CLOSE: // args: 1
    printf("SYS_CLOSE\n");
    break;
  default:
    printf("Unknown condition: %d", *((char *)f->esp));
    break;
  }

  // thread_exit ();

}

/* Reads a byte at user virtual address UADDR.
   UADDR must be below PHYS_BASE. 
   Returns the byte value if successful, -1 if a segfault
   occurred. */

static int
get_user (const uint8_t *uaddr)
   {
       int result;
       asm ("movl $1f, %0; movzbl %1, %0; 1:"
              : "=&a" (result) : "m" (*uaddr));
       return result; 
   }

/* Writes BYTE to user address UDST.
    UDST must be below PHYS_BASE.
    Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
   int error_code;
   asm ("movl $1f, %0; movb %b2, %1; 1:"
           : "=&a" (error_code), "=m" (*udst) : "q" (byte));
   return error_code != -1;
             }
