$(function() {

    $('#simulator-form').on('submit', function(event) {
        event.preventDefault(); // we don't want the page reloading, so things look dynamic (this will be nice when we use d3's transitions)

        var selected_simulator_number = $('#simulator-form input:radio:checked').val();
        var selected_link_bandwidth   = $('#link-bandwidth').val();

        // empty these DOM elements so that new things can be added in
        var simulation_output     = $('#simulation-output').empty();
        var simulation_parameters = $('#simulation-parameters').empty();

        var task_details_table      = $('#task-details-table');

        // empty the table body since we will add new simulation data in
        var task_details_table_body = $('#task-details-table > tbody').empty();

        $('#chart').css('display', 'block');
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
            url: '/run',
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

                // get the data ready to be used with D3's stacked layout
                prepare_for_graph(response.task_data);
                /*console.log(response.task_data);*/

                // generate the graph
                generate_graph(response.task_data);
            }
        });
    });
});

/**
 * Format a number to Five decimal places. At the moment, 5 seems like a good number.
 * Anything less will make some start times look like 0.00 when it's something
 * like 0.00004
 */
var toFiveDecimalPlaces = d3.format('.5f');

/**
 * When we receive the data as a json array, the keys mentioned in the function
 * each have a start and end time value. D3's stack layout needs the difference
 * between these two values so we take care of that here.
 */
function prepare_for_graph(data) {
    var keys = ['read', 'compute', 'write', 'whole_task'];

    data.forEach(function(task, i) {
        task.start = task['whole_task'].start;
        keys.forEach(function(k) {
            task[k] = task[k].end - task[k].start;
        });
    });
}

/**
 * Helper function used to get the position of the mouse within the browser window
 * so that we can have nice moving tooltips. The input is the DOM element we are
 * interested in (in this case the #chart element).
 */
function getOffset(el) {
  const rect = el.getBoundingClientRect();
  return {
    left: rect.left + window.scrollX,
    top: rect.top + window.scrollY
  };
}

function generate_graph(data) {
    // Chart params
    var chart_height = 400; // this should match the CSS file (later we can change it so that d3 dynamically grabs the value)
    var chart_width  = 800; // this should match the CSS file (later we can change it so that d3 dynamically grabs the value)
    var svg_height   = 400;
    var padding      = 60;

    var read_color    = '#cbb5dd';
    var compute_color = '#f7daad';
    var write_color   = '#abdcf4';
    var colors        = [read_color, compute_color, write_color];

    // Save Reference to the chart div and tooltip div
    var chart = document.getElementById('chart');

    var tooltip                         = d3.select('#tooltip');
    var tooltip_task_id                 = d3.select('#tooltip-task-id');
    var tooltip_task_operation          = d3.select('#tooltip-task-operation');
    var tooltip_task_operation_duration = d3.select('#tooltip-task-operation-duration');

    // Create the svg element
    var svg = d3.select('#chart')
        .append('svg')
        .attr('width', chart_width)
        .attr('height', svg_height);

    // Create scales
    var x_scale = d3.scaleLinear()
        .domain([0, d3.max(data, function (d) {
            return d['read'] + d['compute'] + d['write'] + d['start'];
        })])
        .range([padding, chart_width - padding]);

    var task_ids = [];
    data.forEach(function(task) {
        task_ids.push(task['task_id']);
    });

    var y_scale = d3.scaleBand()
        .domain(task_ids)
        .range([chart_height - padding, padding])
        .padding(0.1);

    // Stack layout
    var stack = d3.stack().keys([
        'read', 'compute', 'write'
    ]);

    var stack_data = stack(data);

    // Groups
    var groups = svg.selectAll('g')
        .data(stack_data)
        .enter()
        .append('g')
        .attr('class', function(d) {
            return d.key;
        })
        .style('fill', function(d, i) {
            return colors[i];
        });

    // Add the data
    groups.selectAll('rect')
      .data(function(d) {
          return d;
      })
      .enter()
      .append('rect')
      .attr('x', function(d) {
          return x_scale(d[0] + d['data'].start);
      })
      .attr('y', function(d) {
          return y_scale(d['data'].task_id);
      })
      .attr('height', function(d) {
          return y_scale.bandwidth();
      })
      .attr('width', function(d) {
          return x_scale(d[1]) - x_scale(d[0]);
      })
      .on('mouseover', function() {
        tooltip.style('display', 'inline');

        d3.select(this)
          .attr('stroke', 'gray')
          .style('stroke-width', '1');
      })
      .on('mousemove', function(d) {
        var offset = getOffset(chart);
        var x = d3.event.pageX - offset.left + 20;
        var y = d3.event.pageY - offset.top - 30; // 20 seems to account for the padding of the chart-block

        tooltip.style('left', x + 'px')
          .style('top', y + 'px');

        tooltip_task_id.text('TaskID: ' + d.data.task_id);

        var parent_group = d3.select(this.parentNode).attr('class');

        if (parent_group == 'read') {
          tooltip_task_operation.text('Read Input');
        } else if (parent_group == 'compute') {
          tooltip_task_operation.text('Computation');
        } else if (parent_group == 'write') {
          tooltip_task_operation.text('Write Output');
        }

        var duration = toFiveDecimalPlaces(d[1] - d[0]);
        tooltip_task_operation_duration.text('Duration: ' + duration + 's');

      })
      .on('mouseout', function(d) {
        tooltip.style('display', 'none');

        d3.select(this)
          .attr('stroke', 'none')
      });

    // Create axis
    var x_axis = d3.axisBottom(x_scale);
    svg.append('g')
        .attr('class', 'x-axis')
        .attr('transform',
            'translate(0,' + (chart_height - padding) + ')')
        .call(x_axis);

    var y_axis = d3.axisLeft(y_scale);
    svg.append('g')
        .attr('class', 'y-axis')
        .attr('transform',
            'translate(' + padding + ',0)')
        .call(y_axis);

    // Create x-axis label
    svg.append('text')
        .attr('transform',
            'translate(' + (chart_width / 2) + ' ,' + (chart_height - 10) + ')')
        .style('text-anchor', 'middle')
        .attr('font-size', 10+'px')
        .attr('fill', 'gray')
        .text('Time (seconds)');

     // Create y-axis label
    svg.append('text')
        .attr('transform',
            'rotate(-90)')
        .attr('y', -3)
        .attr('x', 0 - (chart_height / 2))
        .attr('dy', '1em')
        .attr('text-anchor', 'middle')
        .attr('font-size', 10+'px')
        .attr('fill', 'gray')
        .text('Workflow Task');

}
