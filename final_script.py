#!/usr/bin/env python

import sys
import angr
import pyvex

#finds the last jump address for the main while loop
def findJumpAddr(loopList, p):

    jump_addr = 0

    for loop in loopList:
        for block in loop.body_nodes:
            irsb = p.factory.block(block.addr).vex
            if(irsb.jumpkind == 'Ijk_Boring'):
                jump_addr = max(jump_addr, irsb.addr)
            
    print("Found jump address: %s" % hex(jump_addr))
    return jump_addr


#checks if a symbolic variable is completely symbolic
def fully_symbolic(state, variable):
    
    for i in range(state.arch.bits):
        if not state.solver.symbolic(variable[i]):
            return False
    return True

#finds an exploitable state by checking whether the pc is symbolic in an unconstrained path
def findExploitableState(sm):
	exploitable_state = None
	while exploitable_state is None:
	     print(sm)
	     sm.step()
	     if len(sm.unconstrained) > 0:
	         print("found some unconstrained states, checking exploitability")
	         for u in sm.unconstrained:
	             if fully_symbolic(u, u.regs.pc):
	                 exploitable_state = u
	                 break

	#         # no exploitable state found, drop them
	         sm.drop(stash='unconstrained')

	print("found a state which looks exploitable")
	return exploitable_state

#A function which does nothing for our hook
def nothing(state):
    pass

def main(argv):
	if(len(argv) != 1):
		print("Invalid number of command line arguments. Please provide only one binary")
		exit()

	p = angr.Project(argv[-1], auto_load_libs=False)
	cfg = p.analyses.CFGFast(fail_fast=True)

	vulnerable_function = 'doit'
	has_loop = True

	if argv[-1] == 'simplest':
		has_loop = False
		vulnerable_function = 'vuln'

	'''
	get addresses of the main and doit functions in binary.

	We extract the address of the main function so that we can narrow our loop search. 
	Our goal is to search for the last jump instruction in the loop to prevent a state explosion

	The doit function is our vulnerable function, where we will generate a crash
	'''

	main_func = cfg.kb.functions.function(name="main")
	target_func = cfg.kb.functions.function(name=vulnerable_function)

	if has_loop == True:
		#use the LoopFinder analysis technique to find the loop
		lf = p.analyses.LoopFinder([main_func])
		assert len(lf.loops) == 1

		#find the last jump address in the main loop and add a hook for the simulation manager to avoid it
		jump_addr = findJumpAddr(lf.loops, p)
		p.hook(jump_addr, nothing, length=4)

	

	#set up the simulation manager
	extras = {angr.options.REVERSE_MEMORY_NAME_MAP, angr.options.TRACK_ACTION_HISTORY}
	init_state = p.factory.entry_state(add_options=extras)
	sm = p.factory.simulation_manager(init_state, save_unconstrained=True)

	'''
	We wish to find an input which will crash the program. For this to occur, it must find a path to the vulnerable function
	'''
	sm.explore(find=target_func.addr)


	ep = findExploitableState(sm)		
	print(repr(ep.posix.dumps(0)))

if __name__ == "__main__":
    main(sys.argv[1:])