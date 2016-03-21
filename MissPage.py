import numpy

VISIT = 10
SIZE = 2
PAGE = 6
MEM = 4

if __name__ == '__main__':
    visitPage = numpy.random.randint(0, PAGE, VISIT)

    visitTime = dict()
    lastTime = -1
    for i in xrange(VISIT):
        page = visitPage[i]
        if page in visitTime:
            visitTime[page] = i
            print "hit", page
        else:
            if i - lastTime > SIZE:
                tmp = filter(lambda x: x[1] >= lastTime, visitTime.items())
                visitTime = dict(map(lambda x: (x[0], 0), tmp))
                visitTime[page] = i
                print "miss ", page, " : ", visitTime.keys()
                lastTime = i
            else:
                visitTime[page] = i
                print "miss ", page, " : ", visitTime.keys()
                lastTime = i
                if (len(visitTime) > MEM):
                    tmp = visitTime.items()
                    tmp.sort(lambda x, y: x[1] - y[1])
                    visitTime = dict(tmp[1:])