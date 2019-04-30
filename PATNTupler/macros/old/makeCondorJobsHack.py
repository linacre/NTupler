import os

for iJob in range (0, 511):

	f = open("job_impacts_%d.condor" % iJob, 'w')

	f.write("Universe                = vanilla\n")
	f.write("Executable              = job_impacts_%d.sh\n" % iJob)
	f.write("Log                     = logs/job_impacts_%d.log\n" % iJob)
	f.write("Output                  = logs/job_impacts_%d.out\n" % iJob)
	f.write("Error                   = logs/job_impacts_%d.err\n" % iJob)
	f.write("Request_memory          = 1 GB\n")
	f.write("should_transfer_files   = YES\n")
	f.write("when_to_transfer_output = ON_EXIT_OR_EVICT\n")
	f.write("#\n")
	f.write("Getenv                  = True\n")
	f.write("Queue\n")

	f.close()

	print("condor_submit job_impacts_%d.condor" % iJob)
	# os.system("condor_submit job_impacts_%d.condor" % iJob)