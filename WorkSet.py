addr = [4, 3, 0, 2, 2, 3, 1, 2, 4, 2, 4, 0, 3]
visit_list = [0, 0, 0, 0, 0]
t = 4

for i in range(0, len(addr)):
	if visit_list[addr[i]] == 0:
		print("[" + repr(i) + "] Miss")
	else:
		print("[" + repr(i) + "] Hit")

	for j in range(0, len(visit_list)):
		if visit_list[j] != 0:
			visit_list[j] = visit_list[j] + 1
			if visit_list[j] == 5:
				visit_list[j] = 0

	visit_list[addr[i]] = 1

	workset = []
	for j in range(0, len(visit_list)):
		if visit_list[j] != 0:
			workset.append(j)

	print "Workset is " + repr(workset)
	print ''