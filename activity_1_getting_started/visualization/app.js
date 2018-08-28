var express        = require("express"),
    app            = express(),
    bodyParser     = require("body-parser"),
    methodOverride = require("method-override"),
    au             = require('ansi_up'),
    {exec}         = require("child_process"),
    fs             = require('fs'),
    xml2js         = require('xml2js');

app.set("view engine", "ejs");
app.use(express.static("public"));
app.use(bodyParser.urlencoded({extended: true}));
app.use(bodyParser.json());
app.use(methodOverride("_method"));

var ansi_up     = new au.default;
var xml_parser  = new xml2js.Parser();
var xml_builder = new xml2js.Builder({xmldec:{ 'version': '1.0'},
                                      doctype: {'sysID': 'http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd'}
                                    });

app.get("/", function(req, res) {
    res.render("index");
});

app.post("/run", function(req, res) {
    var PATH_PREFIX = '/home/wrench/wrench-pedagogic-modules/' +
        'activity_1_getting_started/';

    var SIMULATOR              = PATH_PREFIX + (req.body.simulator_number == 1 ? 'simulator_remote_storage' : 'simulator_local_storage');
    var PLATFORM_FILE          = PATH_PREFIX + 'platform_files/platform.xml';
    var MODIFIED_PLATFORM_FILE = PATH_PREFIX + 'platform_files/modified_platform.xml';
    var WORKFLOW_FILE          = PATH_PREFIX + 'workflow_files/workflow.dax';

    var LOGGING = [
        '--log=root.thresh:critical',
        '--log=wms.thresh:debug',
        '--log=simple_wms.thresh:debug',
        '--log=simple_wms_scheduler.thresh:debug',
        '--log="root.fmt:[%d][%h:%t]%e%m%n"'
    ];

    var simulation_parameters = {
      executable             : SIMULATOR,
      platform_file          : PLATFORM_FILE,
      modified_platform_file : MODIFIED_PLATFORM_FILE,
      workflow_file          : WORKFLOW_FILE,
      logging                : LOGGING.join(' '),
      link_bandwidth         : req.body.link_bandwidth,

      getRunCommand: function() {
        return this.executable + ' ' + this.modified_platform_file + ' ' + this.workflow_file + ' ' + this.logging;
      }
    };

    run_and_send_response(simulation_parameters, res);
});

app.listen(3000, function() {
    console.log("Server is running!");
});


function run_and_send_response(simulation_parameters, res) {
  fs.readFile(simulation_parameters.platform_file, generateReadFileCallback(simulation_parameters, res));
}

function generateReadFileCallback(simulation_parameters, res) {
  return function(err, data) {
    xml_parser.parseString(data, generateParseStringCallback(simulation_parameters, res));
  }
}

function generateParseStringCallback(simulation_parameters, res) {
  return function(err, result) {

    console.log(xml_builder.buildObject(result));
    console.log(result.platform.zone[0].link[0]['$'].bandwidth);

    result.platform.zone[0].link[0]['$'].bandwidth = simulation_parameters.link_bandwidth + "MBps";
    console.log(xml_builder.buildObject(result));

    fs.writeFileSync(simulation_parameters.modified_platform_file, xml_builder.buildObject(result));

    var cmd = simulation_parameters.getRunCommand();

    console.log("Executing cmd: " + cmd);
    exec(cmd, function(err, stdout, stderr) {
      if (err) {
          console.error('We encountered a problem');
          console.log(stderr);
          return;
      }

      console.log(stderr);

      var find = '</span>';
      var re = new RegExp(find, 'g');

      res.json({
          "simulation_output": ansi_up.ansi_to_html(stderr).replace(re, '<br>' + find),
          "task_data": JSON.parse(fs.readFileSync("/home/wrench/workflow_data.json"))
      });
    });
  }
}
