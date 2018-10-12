$(function() {

    $('#simulator-form').on('submit', function(event) {

        var run_button = document.getElementById("run-button");

        run_button.disabled = true;
        setTimeout(function(){
            run_button.disabled = false;
        },5000);



        event.preventDefault(); // we don't want the page reloading, so things look dynamic (this will be nice when we use d3's transitions)

        var num_nodes = $('#num-nodes').val();
        var num_cores_per_node   = $('#num-cores').val();
        var ram_required = $('#ram-required').is(':checked') ? 1 : 0;

        // empty these DOM elements so that new things can be added in
        var simulation_output     = $('#simulation-output').empty();

        var task_details_table      = $('#task-details-table');

        // empty the table body since we will add new simulation data in
        var task_details_table_body = $('#task-details-table > tbody').empty();

        $('#workflow-execution-chart').css('display', 'block');
        $('#host-utilization-chart').css('display', 'block');
        $('svg').remove(); // remove the graph, since we will append a new one to the chart

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
            url: '/run/activity_2',
            method: 'POST',
            contentType: 'application/json',
            data: JSON.stringify({num_nodes: num_nodes,
                num_cores_per_node: num_cores_per_node,
                ram_required: ram_required}),
            success: function(response) {

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



                generate_host_utilization_graph(response.task_data);


                // generate the graph
                generate_workflow_execution_graph(response.task_data);
            }
        });
    });
});
