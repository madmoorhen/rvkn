.section .text.init
.option norvc

# Initial entry point (machine mode)
.type start, @function
.global start
_start:
	
  # Set global pointer
.option push
.option norelax
	la gp, __global_pointer
.option pop

  # Turn off paging
	csrw satp, zero

  # Set stack pointer
	la sp, __stack_top
	
  # Zero out BSS
	la t0, __bss_start
	la t1, __bss_end
bss_clear:
	sd zero, (t0)
	addi t0, t0, 8
	bltu t0, t1, bss_clear

  # Delegate all machine-level interrupts and exceptions to supervisor mode
  li t0, 0xffff
  csrs medeleg, t0
  li t0, 0xffff
  csrs mideleg, t0

  # Enable all supervisor-level interrupts
  li t0, 0x2222
  csrs sie, t0

  # Disable PMP (Physical Memory Protection)
  li t0, 0x3fffffffffffff
  csrw pmpaddr0, t0
  csrw pmpcfg0, 0xf

  # Ensure hardware updates A and D (Accessed and Dirty) bits for PTEs (Page
  # Table Entries), and, if possible, allow supervisor mode control of counter
  # threshold
  li t0, 0xa000000000000000
  csrs menvcfg, t0
  csrs mcounteren, 0x7 # Enable cycle, time, and instret for supervisor

  # Get some machine-level only information, and put it in argument registers,
  # ready to "return" to kmain in supervisor mode
  csrr a0, mhartid # arg0 = mhartid
  mv a2, a1 # arg2 = dtb ptr
  csrr a1, misa # arg1

  # MPP (Machine interrupt Previous Permission) to S (Supervisor)
  li t0, 0x800
  csrs mstatus, t0
  li t0, 0x1000
  csrc mstatus, t0

  # Disable supervisor interrupts
  csrc mstatus, 0x2
	
  # Machine interrupt return address is kmain
	la t0, kmain
	csrw mepc, t0

  # Return to supervisor mode
  mret
  
  # Should be unreachable
end: j end
