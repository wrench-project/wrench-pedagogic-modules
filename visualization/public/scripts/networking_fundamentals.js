"use strict";

$(function() {

    $('#simulator-form').on('submit', function(event) {
        // we don't want the page reloading, so things look dynamic (this will be nice when we use d3's transitions)
        event.preventDefault();
        disableRunSimulationButton();

        // Upon submission of the form, a POST request containing the user's desired parameters
        // is sent to the node server, where the simulation will be executed with those parameters.
        // Then a response with simulation data is received. The data is parsed, and rendered on the
        // screen. 
        $.ajax({
            url: '/run/networking_fundamentals',
            method: 'POST',
            contentType: 'application/json',
            data: JSON.stringify(
                {
                    file_sizes: $('#file-sizes').val()
                }),

                success: function(response) {

                    // Add the new simulation output into the "Simulation Output" section
                    $("#simulation-output").empty()
                        .append(response.simulation_output);
                }
        });
    });
});
