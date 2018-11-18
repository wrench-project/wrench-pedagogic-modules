# for testing activity 3 options

from subprocess import Popen, PIPE, STDOUT
from collections import namedtuple

Platform_Configuration = namedtuple('Platform_Configuration', ['cores', 'ram_in_gb', 'bandwidth_in_mbps'])

run_command = "./activity_simulator {0} {1} {2} {3} --log=root.thresh:critical"

possible_configurations = {
    'base'           : Platform_Configuration(2, 16, 100),
    '2x_cores_10x_bw': Platform_Configuration(4, 16, 1000),
    '2x_ram_10x_bw'  : Platform_Configuration(2, 32, 1000),
    '2x_cores_2x_ram': Platform_Configuration(4, 32, 100)
}

# 2GB input files
input_file_size_in_mb = 2000

print("---------------------------------------------------------------------")

# run simulation with each option, then print simulated and estimated execution times and utilization
for configuration_name, configuration in possible_configurations.items():
    print(configuration_name)
    print(configuration)

    # compute estimated execution time
    estimated_execution_time = 0

    if configuration_name == 'base':
        estimated_execution_time = (3 * input_file_size_in_mb / configuration.bandwidth_in_mbps) + (3 * 100) + 10 + (0.003 / 100)

    elif configuration_name == '2x_cores_10x_bw':
        estimated_execution_time = (3 * input_file_size_in_mb / configuration.bandwidth_in_mbps) + (3 * 100) + 10 + (0.003 / 100)

    elif configuration_name == '2x_ram_10x_bw':
        estimated_execution_time = (3 * input_file_size_in_mb / configuration.bandwidth_in_mbps) + (2 * 100) + 10 + (0.003 / 100)

    elif configuration_name == '2x_cores_2x_ram':
        estimated_execution_time = (3 * input_file_size_in_mb / configuration.bandwidth_in_mbps) + 100 + 10 + (0.003 / 100)

    print("estimated_execution_time: {}".format(estimated_execution_time))

    # execute the simulation
    wrench_process = Popen(run_command.format(*configuration, input_file_size_in_mb), shell=True, stdout=PIPE)
    wrench_process.wait()

    simulated_execution_time = float(str(wrench_process.communicate()[0], 'utf-8').strip())
    print("simulated_execution_time: {}".format(simulated_execution_time))

    # compute utilization (3 100TFlop tasks + 1 10TFlop task on 1TFlop core(s) so 310 is always the numerator)
    utilization = 310 / (configuration.cores * simulated_execution_time)
    print("utilization: {}".format(utilization))

    print("---------------------------------------------------------------------")
