
with open("tool/data", "r") as f:
    lines = f.readlines()
with open("tool/teapot.cc", "w") as f:
    firstf = True
    firsfv = True
    f.write("#include \"Space.hpp\"\n")
    f.write("Vec3D vs[] = {")
    vlen = 0
    flen = 0
    for l in lines:
        arr = l.split()
        if len(arr) != 4:
            continue
        if arr[0] == 'f':
            # print("is triangle")
            flen += 1
            if firstf:
                f.write("};\n")
                print("vertex end")
                firstf = False
                f.write("int fs[][3] = {")
            f.write("{%s, %s, %s},\n"%(arr[1], arr[2], arr[3]))
            pass
        elif arr[0] == 'v':
            # print("is vertex")
            vlen += 1
            
            f.write("{%f, %f, %f},\n"%(100 *  float(arr[1]),-100 *  float(arr[2]),100 * float(arr[3])))
            pass
        else:
            print("unknown format")
        
        
    f.write("};\n")
    f.write("int vlen = %i;\n"%vlen)
    f.write("int flen = %i;\n"%flen)
