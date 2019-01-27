const express    = require("express"),
      app        = express(),
      bodyParser = require("body-parser"),
      fs         = require("fs"),
      keys       = require("./keys.js");

app.use(bodyParser.urlencoded({extended: true}));
app.use(bodyParser.json());

// data posting route
app.post("/submit_data", function(req, res) {
  let time_now = new Date().toLocaleString("en-US", {timeZone: "Pacific/Honolulu"});

  if (req.body.key == keys.dataServer.key) {
    console.log(time_now + ": received data");
    console.log(req.body.data);

    // DATA_FILE will reside on the host machine, and its path will be
    // mounted into the docker container at /home/data_server/wrench-pedagogic-modules/data_server
    const DATA_FILE = "./data/data_file.json";
    fs.readFile(DATA_FILE, function(err, data) {
        let current_json_data = [];
        let received_data = req.body.data;

        // if the file was able to be read, update the json list with new data
        if (!err) {
            current_json_data = JSON.parse(data);
            current_json_data.push(received_data);
        // if the file was not able to be read, append the new data to the empty json list
        } else {
            current_json_data.push(received_data);
        }

        // write the resulting json object to the file (overwritting the old file);
        // cannot simply append since we are writing as json
        fs.writeFile(DATA_FILE, JSON.stringify(current_json_data), function(err) {
            if (err) {
                console.log("app.post('/submit_data') callback: there was a problem writing the json file");
            }
        });
    });

    res.status(201).end();

  } else {
      console.log(time_now + ": invalid post_request received");
      res.status(400).end();
  }
});

app.listen(3001, function() {
    console.log("Data server is running on port 3001");
});
