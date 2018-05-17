# Get list of completed jobs
completed_jobs = jobs(28).subjobs.select(status="completed")
locations = []
for sj in completed_jobs:
    locations.append(sj.id)

from subprocess import call
# Check if file exists on common scratch
for i in locations:
    set_dir =
    "DIRECTORY=\"data/commonscratch/lhcb/colmmurphy/gangadir/workspace/colmmurphy/LocalXML/{d}\"".format(d=i)
    call(set_dir, shell=True)
# If file does exist, then move it to data disk
    call("if [ -d \"$DIRECTORY\" ];", shell=True)
    call("cd \"$DIRECTORY\"", shell=True)
    call("echo 'hello'", shell=True)
# Then delete common scratch file
