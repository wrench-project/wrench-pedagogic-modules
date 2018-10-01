var express        = require("express"),
    app            = express(),
    bodyParser     = require("body-parser"),
    methodOverride = require("method-override"),
    au             = require('ansi_up'),
    {exec, spawnSync}         = require("child_process"),
    fs             = require('fs'),
    xml2js         = require('xml2js');

app.set("view engine", "ejs");
app.use(express.static("public"));
app.use(bodyParser.urlencoded({extended: true}));
app.use(bodyParser.json());
app.use(methodOverride("_method"));

var ansi_up     = new au.default; // WRENCH produces output to the terminal using ansi colors, ansi_up will apply those colors to <span> html elements
var xml_parser  = new xml2js.Parser(); // used to parse the platform.xml file so that we change change certain parameters (in this case link bandwidth)
var xml_builder = new xml2js.Builder({xmldec:{ 'version': '1.0'}, // used to convert the javascript xml object into a string so we can write it back to disk before running the simulation
                                      doctype: {'sysID': 'http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd'}
                                    });

// default route that displays the page with no data and only a form to be filled
app.get("/", function(req, res) {
    res.render("index");
});

/**
 * When the user submits simulation parameters, a POST request is sent to this route and
 * we do the following:
 *  1. get simulation parameters ready
 *  2. read in the platform_file and parse it into a javascript object so that we can set the link bandwidth property to whatever the user input
 *  3. write out a new platform_file as modified_platform_file with the updated link bandwidth
 *  4. have node create a process that runs the WRENCH simulator (which uses the modified_platform_file)
 *  5. save the simulation output so that when can send that back to the client to be rendered in the browser
 *
 */
app.post("/run", function(req, res) {

    const PATH_PREFIX = __dirname.replace('visualization', '');

    const EXECUTABLE             = PATH_PREFIX + (req.body.simulator_number == 1 ? 'simulator_remote_storage' : 'simulator_local_storage');
    const PLATFORM_FILE          = PATH_PREFIX + 'platform_files/platform.xml';
    const MODIFIED_PLATFORM_FILE = PATH_PREFIX + 'platform_files/modified_platform.xml';
    const WORKFLOW_FILE          = PATH_PREFIX + 'workflow_files/workflow.dax';
    const LINK_BANDWIDTH         = req.body.link_bandwidth;

    // additional WRENCH arguments that filter simulation output (We only want simulation output from the WMS in this activity)
    const LOGGING = [
        '--log=root.thresh:critical',
        '--log=wms.thresh:debug',
        '--log=simple_wms.thresh:debug',
        '--log=simple_wms_scheduler.thresh:debug',
        '--log="root.fmt:[%d][%h:%t]%e%m%n"'
    ];

    const SIMULATION_ARGS = [MODIFIED_PLATFORM_FILE, WORKFLOW_FILE].concat(LOGGING);
    const RUN_SIMULATION_COMMAND = [EXECUTABLE].concat(SIMULATION_ARGS).join(' ');

    var data = fs.readFileSync(PLATFORM_FILE);

    var parsed_xml;
    xml_parser.parseString(data, function(err, result) {
      parsed_xml = result;
    });

    console.log('\nPlatform File Original Contents');
    console.log('===============================');
    console.log(xml_builder.buildObject(parsed_xml) + '\n');

    console.log('\nUpdating Link Bandwidth to ' + LINK_BANDWIDTH + ' MBps');
    console.log('===================================');
    parsed_xml.platform.zone[0].link[0]['$'].bandwidth = LINK_BANDWIDTH + "MBps";
    fs.writeFileSync(MODIFIED_PLATFORM_FILE, xml_builder.buildObject(parsed_xml));
    console.log(xml_builder.buildObject(parsed_xml) + '\n');

    console.log('\nRunning Simulation');
    console.log('===================');
    console.log('Executing command: ' + RUN_SIMULATION_COMMAND);
    var simulation_process = spawnSync(EXECUTABLE, SIMULATION_ARGS);

    if (simulation_process.status != 0) {
      console.log('Something went wrong with the simulation. Possibly check arguments.');
      console.log(simulation_process.stderr.toString());
    } else {
      var simulation_output = simulation_process.stderr.toString();
      console.log(simulation_output);

      /**
       * The simulation output uses ansi colors and we want these colors to show up in the browser as well.
       * Ansi up will take each line, make it into a <span> element, and edit the style so that the text color
       * is whatever the ansi color was. Then the regular expression just adds in <br> elements so that
       * each line of output renders on a separate line in the browser.
       *
       * The simulation output and the workflowtask data are sent back to the client (see public/scripts.js)
       */
      var find = '</span>';
      var re = new RegExp(find, 'g');

      res.json({
          "simulation_output": ansi_up.ansi_to_html(simulation_output).replace(re, '<br>' + find),
          "task_data": JSON.parse(fs.readFileSync(__dirname + '/workflow_data.json'))
      });
    }
});

app.listen(3000, function() {
    console.log("Visualization server is running!");
});
