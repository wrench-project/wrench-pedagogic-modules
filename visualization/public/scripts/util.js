// Disable "Run Simulation" button for 5 seconds after submit
function disableRunSimulationButton() {
    let run_simulation_button = $("#run-button");

    run_simulation_button.attr("disabled", "disabled");
    setTimeout(function() {
        run_simulation_button.removeAttr("disabled");
    }, 5000);
}

// Fill Workflow Task Data table with data obtained from the simulation run
function populateWorkflowTaskDataTable(data) {

    // Reveal the Workflow Data table
    $("#task-details-table").css('display', 'block');

    let task_details_table_body = $("#task-details-table > tbody").empty();

    const TASK_DATA = Object.assign([], data).sort(function(lhs, rhs) {
        return parseInt(lhs.task_id.slice(4)) - parseInt(rhs.task_id.slice(4));
    });

    TASK_DATA.forEach(function(task) {

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
}