$(function() {

    $('#simulator-form').on('submit', function(event) {
        event.preventDefault();

        var selected_simulator_number = $('#simulator-form input:radio:checked').val();
        var selected_link_bandwidth   = $('#link-bandwidth').val();

        var simulation_output     = $('#simulation-output').empty();
        var simulation_parameters = $('#simulation-parameters').empty();

        var task_details_table      = $('#task-details-table');
        var task_details_table_body = $('#task-details-table > tbody').empty();

        $('#chart').css('display', 'block');
        $('svg').remove();

        $.ajax({
            url: '/run',
            method: 'POST',
            contentType: 'application/json',
            data: JSON.stringify({simulator_number: selected_simulator_number,
                                  link_bandwidth: selected_link_bandwidth}),
            success: function(response) {

                simulation_parameters.text(
                  'Platform Link Bandwidth: ' + selected_link_bandwidth + ' MBps | '
                  + 'Storage Service Configuration: '
                  + (selected_simulator_number == 1 ? 'Storage Service on Fafard' : 'Storage Service on Fafard and Jupiter')
                );
                simulation_output.append(response.simulation_output);

                /////////////// generate data table
                task_details_table.css('display', 'block');
                response.task_data.forEach(function(task, i) {

                    var task_id = task['task_id'];

                    var read_start       = toTwoDecimalPlaces(task['read'].start);
                    var read_end         = toTwoDecimalPlaces(task['read'].end);
                    var read_duration    = toTwoDecimalPlaces(read_end - read_start);

                    var compute_start    = toTwoDecimalPlaces(task['compute'].start);
                    var compute_end      = toTwoDecimalPlaces(task['compute'].end);
                    var compute_duration = toTwoDecimalPlaces(compute_end - compute_start);

                    var write_start      = toTwoDecimalPlaces(task['write'].start);
                    var write_end        = toTwoDecimalPlaces(task['write'].end);
                    var write_duration   = toTwoDecimalPlaces(write_end - write_start);

                    var task_duration    = toTwoDecimalPlaces(write_end - read_start);

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

                prepare_for_graph(response.task_data);
                console.log(response.task_data);
                generate_graph(response.task_data);
            }
        });
    });
});

var toTwoDecimalPlaces = d3.format('.5f');

function prepare_for_graph(data) {
    var keys = ['read', 'compute', 'write', 'whole_task'];

    data.forEach(function(task, i) {
        task.start = task['whole_task'].start;
        keys.forEach(function(k) {
            task[k] = task[k].end - task[k].start;
        });
    });
}

function getOffset(el) {
  const rect = el.getBoundingClientRect();
  return {
    left: rect.left + window.scrollX,
    top: rect.top + window.scrollY
  };
}

function generate_graph(data) {
    // Chart params
    var chart_height = 400;
    var chart_width  = 800;
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

        var duration = d[1] - d[0];

        if (duration == d.data.read) {
          tooltip_task_operation.text('Read Input');
        } else if (duration == d.data.compute) {
          tooltip_task_operation.text('Computation');
        } else if (duration == d.data.write) {
          tooltip_task_operation.text('Write Output');
        }

        tooltip_task_operation_duration.text('Duration: ' + toTwoDecimalPlaces(duration) + 's');

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
