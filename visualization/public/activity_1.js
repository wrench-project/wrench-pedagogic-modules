function updateLinkBandwidth() {
    var input_text = $("#link-bandwidth").val();
    if (input_text.length < 4 && parseInt(input_text) >= 1) {
        $(".bandwidth-label").text("Bandwidth: " + input_text + " MBps")
            .css("background-color", "#d3ffe9");

        $("#link-bandwidth").removeClass("is-invalid");
        $("#link-bandwidth").addClass("is-valid");

        setTimeout(function() {
            $(".bandwidth-label").css("background-color", "");
        }, 500);
    } else {
        $(".bandwidth-label").css("background-color", "#ffb7b5");
        $("#link-bandwidth").removeClass("is-valid").addClass("is-invalid");

    }
}

$(function() {

    $('.local-storage-service-label').css('display', 'none');

    $("#remote-storage-service-input").on("click", function() {
        $('.local-storage-service-label').css('display', 'none');
    });

    $("#local-storage-service-input").on("click", function() {
        $('.local-storage-service-label').css('display', 'block');
    });

    $('#link-bandwidth').on('keyup', updateLinkBandwidth);

    $('#simulator-form').on('submit', function(event) {
        event.preventDefault(); // we don't want the page reloading, so things look dynamic (this will be nice when we use d3's transitions)

        var run_button = document.getElementById("run-button");

        run_button.disabled = true;
        setTimeout(function(){
            run_button.disabled = false;
        },5000);

        var selected_simulator_number = $('#simulator-form input:radio:checked').val();
        var selected_link_bandwidth = $('#link-bandwidth').val();

        // empty these DOM elements so that new things can be added in
        var simulation_output     = $('#simulation-output').empty();
        var simulation_parameters = $('#simulation-parameters').empty();

        var task_details_table      = $('#task-details-table');

        // empty the table body since we will add new simulation data in
        var task_details_table_body = $('#task-details-table > tbody').empty();

        $('.chart').css('display', 'block');
        $('#workflow-execution-chart > svg').remove(); // remove the graph, since we will append a new one to the chart

        /**
         * In the current implementation, the gantt chart is removed and then created
         * as a completely new element with the new data. An ajax request is used, however
         * because it would be nice to take advantage of d3's transition api which will
         * show nice smooth transition of the data changing on the graph. So instead of a
         * function call like generate_graph, it would be great to have something like
         * initialize_graph(empty_data) called when the user first sees the page, and then
         * update_graph(new_data) when the user clicks on run simulation.
         *
         * In summary, we first send the simulation parameters selected by the user to node
         * (which is running app.js). After node runs the simulation with those parameters,
         * it will send this client back a response with data from that simulation.
         *
         * Upon successfully receiving that data, we first fill up the 'Workflow Data'
         * table. Then, we do some cleaning of the data (see parse_for_graph(data))
         * and finally generate the graph with that data.
         */
        $.ajax({
            url: '/run/activity_1',
            method: 'POST',
            contentType: 'application/json',
            data: JSON.stringify({simulator_number: selected_simulator_number,
                                  link_bandwidth: selected_link_bandwidth}),
            success: function(response) {

                // show the users what parameters they used for the simulation
                simulation_parameters.text(
                  'Platform Link Bandwidth: ' + selected_link_bandwidth + ' MBps | '
                  + 'Storage Service Configuration: '
                  + (selected_simulator_number == 1 ? 'Storage Service on storage_db.edu' : 'Storage Service on storage_db.edu and hpc.edu')
                );
                simulation_output.append(response.simulation_output);

                // generate the Workflow Data table
                task_details_table.css('display', 'block');

                response.task_data.sort(function(lhs, rhs) {
                    return parseInt(lhs.task_id.slice(4)) - parseInt(rhs.task_id.slice(4));
                });

                response.task_data.forEach(function(task, i) {

                    var task_id = task['task_id'];

                    var read_start       = toFiveDecimalPlaces(task['read'].start);
                    var read_end         = toFiveDecimalPlaces(task['read'].end);
                    var read_duration    = toFiveDecimalPlaces(read_end - read_start);

                    var compute_start    = toFiveDecimalPlaces(task['compute'].start);
                    var compute_end      = toFiveDecimalPlaces(task['compute'].end);
                    var compute_duration = toFiveDecimalPlaces(compute_end - compute_start);

                    var write_start      = toFiveDecimalPlaces(task['write'].start);
                    var write_end        = toFiveDecimalPlaces(task['write'].end);
                    var write_duration   = toFiveDecimalPlaces(write_end - write_start);

                    var task_duration    = toFiveDecimalPlaces(write_end - read_start);

                    task_details_table_body.append(
                      '<tr id=' + task_id + '>'
                      + '<td>' + task_id +'</td>'
                      + '<td>' + read_start +'</td>'
                      + '<td>' + read_end +'</td>'
                      + '<td>' + read_duration +'</td>'
                      + '<td>' + compute_start +'</td>'
                      + '<td>' + compute_end +'</td>'
                      + '<td>' + compute_duration +'</td>'
                      + '<td>' + write_start +'</td>'
                      + '<td>' + write_end +'</td>'
                      + '<td>' + write_duration +'</td>'
                      + '<td>' + task_duration +'</td>'
                      + '</tr>'
                    );
                });



                // generate the graph
                generate_workflow_execution_graph(response.task_data);
            }
        });
    });
});

