const express    = require("express"),
      app        = express(),
      bodyParser = require("body-parser"),
      fs         = require("fs"),
      keys       = require("./keys.js");

app.use(bodyParser.urlencoded({extended: true}));
app.use(bodyParser.json());

// data posting route
app.post("/submit_data", function(req, res) {
  if (req.body.key == keys.dataServer.key) {
    console.log("received data: ");
    console.log(req.body.data);
    console.log("-------------------");

    const DATA_FILE = "data_file.json";
    fs.readFile(DATA_FILE, function(err, data) {
        let current_json_data = [];
        let received_data = req.body.data;

        // if the file was able to be read, update the json list with new data
        if (!err) {
            current_json_data = JSON.parse(data);
            console.log(DATA_FILE + " was read in");
            current_json_data.push(received_data);
            console.log("its new contents will be ");
            console.log(current_json_data);
        // if the file was not able to be read, append the new data to the empty json list
        } else {
            console.log(DATA_FILE + " doesn't exist");
            current_json_data.push(received_data);
        }

        // write the resulting json object to the file (overwritting the old file)
        fs.writeFile(DATA_FILE, JSON.stringify(current_json_data), function(err) {
            if (err) {
                console.log("there was a problem writing the json file");
            }
        });
    });

    res.status(201).end();
  } else {
      console.log("shouldn't be here...");
      res.status(400).end();
  }
});

app.listen(3001, function() {
    console.log("Data server is running on port 3001");
});
