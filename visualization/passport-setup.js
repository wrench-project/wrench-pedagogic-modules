const passport       = require("passport"),
      GoogleStrategy = require("passport-google-oauth20"),
      keys           = require("./keys.js");

// required function that specifies which user information should
// be stored in a session, in this case just email
passport.serializeUser(function(email, done) {
  done(null, email);
});

// required function that specifies what information will
// be attached to req.user
passport.deserializeUser(function(email, done) {
  done(null, email);
});

// use the google oauth2 strategy to verify identity
passport.use(
  new GoogleStrategy({
    callbackURL: "/google/redirect",
    clientID: keys.google.clientID,        // load from external file
    clientSecret: keys.google.clientSecret // load from external file
  }, function(accessToken, refreshToken, profile, done) {
    // of the information sent back by google, we only need
    // the email (email scope is specified in the route for the callbackURL)
    let email = profile['emails'][0]['value'];
    done(null, email);
  })
);
