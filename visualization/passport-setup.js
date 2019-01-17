const passport       = require("passport"),
      GoogleStrategy = require("passport-google-oauth20");

passport.serializeUser(function(email, done) {
  done(null, email);
});

passport.deserializeUser(function(email, done) {
  done(null, email);
});

passport.use(
  new GoogleStrategy({
    callbackURL: "/google/redirect",
    clientID: ,
    clientSecret: 
  }, function(accessToken, refreshToken, profile, done) {
    let email = profile['emails'][0]['value'];
    done(null, email);
  })
);
