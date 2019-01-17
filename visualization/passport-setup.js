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
    clientID: "181159033486-i50jrj7cs2vadnophdvi8cump2d3slcn.apps.googleusercontent.com",
    clientSecret: "1aYmYt0-HeSuMo12N44QAh9D"
  }, function(accessToken, refreshToken, profile, done) {
    let email = profile['emails'][0]['value'];
    done(null, email);
  })
);
