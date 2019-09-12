"use strict";

$(function() {

    // Initially hide the label that says "Local Storage Service"
    $('.local-storage-service-label').css('display', 'none');

    // Hide or display the "Local Storage Service" label
    $("#remote-storage-service-input").on("click", function() {
        $('.local-storage-service-label').css('display', 'none');
    });

    $("#local-storage-service-input").on("click", function() {
        $('.local-storage-service-label').css('display', 'block');
    });

    // Update link bandwidths labels on SVG live as user types into
    // the form
    $('#compute-speed').on('keyup', function() {

        let compute_speed_input_el = $(this)
        let compute_speed_input_value = parseInt(compute_speed_input_el.val());
        let compute_speed_label_el = $(".computespeed-label");

        // highlight the compute speed label using green on the SVG for half a second
        //to indicate to the user that a valid number has been entered
        if (compute_speed_input_value >= 1 && compute_speed_input_value <= 999) {
            compute_speed_label_el.text("Speed: " + (compute_speed_input_value) + " TFlop/sec")
                .css("background-color", "#d3ffe9");

            compute_speed_input_el.removeClass("is-invalid")
                .addClass("is-valid");

            setTimeout(function() {
                if (compute_speed_label_el.css("background-color") == "rgb(211, 255, 233)") {
                    compute_speed_label_el.css("background-color", "");
                }
            }, 500);

        // highlight the bandwidth label using red on the SVG to indicate to the user
        // that an invalid number has been entered
        } else {
            compute_speed_label_el.css("background-color", "#ffb7b5");

            compute_speed_input_el.removeClass("is-valid")
                .addClass("is-invalid");
        }
    });

    $('#simulator-form').on('submit', function(event) {
        // we don't want the page reloading, so things look dynamic (this will be nice when we use d3's transitions)
        event.preventDefault();
        disableRunSimulationButton();

        // Display the chart element, which will enlarge the workflow execution block
        $('.chart').css('display', 'block');

        // After each submission, remove the "old" chart, because we will generate a new one
        $('#workflow-execution-chart > svg').remove();

        // Upon submission of the form, a POST request containing the user's desired parameters
        // is sent to the node server, where the simulation will be executed with those parameters.
        // Then a response with simulation data is received. The data is parsed, and rendered on the
        // screen. 
        $.ajax({
            url: '/run/workflow_execution_fundamentals',
            method: 'POST',
            contentType: 'application/json',
            data: JSON.stringify(
                {
                    // simulator_number: $('#simulator-form input:radio:checked').val(),
                    compute_speed: $('#compute-speed').val()
                }),

                success: function(response) {

                    // Add the new simulation output into the "Simulation Output" section
                    $("#simulation-output").empty()
                        .append(response.simulation_output);

                    generate_workflow_execution_graph(response.task_data);

                    populateWorkflowTaskDataTable(response.task_data);
                }
        });
    });
});
