# for testing activity 3 options

from subprocess import Popen, PIPE, STDOUT

class Activity3_Simulator:
    run_command = "./activity_simulator {0} {1} {2} {3} --log=root.thresh:critical"

    def __init__(self, num_cores, ram_in_gb, bandwidth_in_mbps, input_file_size_in_mb, estimated_execution_time_func):
        self.num_cores = num_cores
        self.ram_in_gb = ram_in_gb
        self.bandwidth_in_mbps = bandwidth_in_mbps
        self.input_file_size_in_mb = input_file_size_in_mb
        self.estimated_execution_time_func = estimated_execution_time_func

    def run(self):
        wrench_process = Popen(Activity3_Simulator.run_command.format(self.num_cores, self.ram_in_gb, self.bandwidth_in_mbps, self.input_file_size_in_mb), shell=True, stdout=PIPE)
        wrench_process.wait()

        return float(str(wrench_process.communicate()[0], 'utf-8').strip())

    def get_estimated_execution_time(self):
        return self.estimated_execution_time_func()

    def get_estimated_utilization(self):
        return 310 / (self.num_cores * self.get_estimated_execution_time())

    def __str__(self):
        return "Configuration: cores={0}, ram_in_gb={1}, bandwidth_in_mbps={2}, input_file_size_in_mb={3}".format(self.num_cores, self.ram_in_gb, self.bandwidth_in_mbps, self.input_file_size_in_mb)

input_file_size_in_mb = 2000

simulators = {
    'base'           : Activity3_Simulator(2, 16, 100, input_file_size_in_mb, lambda : (3 * input_file_size_in_mb / 100) + (3 * 100) + 10 + (0.003 / 100)),
    '2x_cores_10x_bw': Activity3_Simulator(4, 16, 1000, input_file_size_in_mb, lambda : (3 * input_file_size_in_mb / 1000) + (3 * 100) + 10 + (0.003 / 100)),
    '2x_ram_10x_bw'  : Activity3_Simulator(2, 32, 1000, input_file_size_in_mb, lambda : (3 * input_file_size_in_mb / 1000) + (2 * 100) + 10 + (0.003 / 100)),
    '2x_cores_2x_ram': Activity3_Simulator(4, 32, 100, input_file_size_in_mb, lambda : (3 * input_file_size_in_mb / 100) + 100 + 10 + (0.003 / 100))
}


print("---------------------------------------------------------------------")

# run simulation with each option, then print simulated and estimated execution times and utilization
for simulator_name, simulator in simulators.items():
    print("Upgrade Option: " + simulator_name)
    print(str(simulator))

    # compute estimated execution time
    print("estimated_execution_time: {}".format(simulator.get_estimated_execution_time()))

    # execute the simulation
    simulated_execution_time = simulator.run()
    print("simulated_execution_time: {}".format(simulated_execution_time))

    # compute utilization
    print("estimated_utilization: {}".format(simulator.get_estimated_utilization()))

    print("---------------------------------------------------------------------")
