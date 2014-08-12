import os

binDir = os.getenv("LQANALYZER_DIR")+ "/bin/"
fr = open(binDir + 'Branch.txt', 'r')
sline=0
for line in fr:
    if sline ==0:
        print "Local code uses LQanalyser version : " +  line
        print "Look up code at https://github.com/jalmond/LQanalyzer/tree/" + line
    if sline == 1:
        print "..."
    sline=sline+1
    
   

if os.path.exists("LQRun/Macros/"):
    print "Cleaning up directory that failed to be removed by git merge"
    os.system("rm -r LQRun/Macros/")
if os.path.exists("LQRun/job_output/"):
    print "Cleaning up directory that failed to be removed by git merge"
    os.system("rm -r LQRun/job_output/")
if os.path.exists("LQRun/runJob_1.C"):
    print "Cleaning up file that failed to be removed by git merge"
    os.system("rm LQRun/runJob_1.C")
if os.path.exists("LQCycle/"):
    print "Cleaning up directory that failed to be removed by git merge"
    os.system("rm -r LQCycle/")


junedir = os.getenv("LQANALYZER_LIB_PATH")+ "/June/"
if not os.path.exists(junedir):
    os.system("mkdir " + junedir)
    print "using June branch for first time: All codes are being recompiled"
    os.system("source bin/make_clean_newbranch.sh")
    

fakelib = os.getenv("LQANALYZER_LIB_PATH") + "libHNCommonLeptonFakes.so"

if not os.path.exists(fakelib):
    os.system("source bin/make_fake_lib.sh")

rocherlib = os.getenv("LQANALYZER_LIB_PATH") + "librochcor2012.so"

if not os.path.exists(rocherlib):
        os.system("source bin/make_rocher_lib.sh")
        
