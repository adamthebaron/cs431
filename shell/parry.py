import lldb
import shlex

def parry(debugger, command, result, dict):
	args = shlex.split(command)
	va = lldb.frame.FindVariable(args[0])
	for i in range(0, int(args[1])):
		print va.GetChildAtIndex(i, 0, 1)