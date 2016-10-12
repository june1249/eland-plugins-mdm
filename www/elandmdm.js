
var exec = require('cordova/exec');

var PLUGIN_NAME = 'MDM';

var MDMPlugin = {
  check_app: function(sucess, failure) {
    exec(sucess, failure, PLUGIN_NAME, 'check_app', []);
  }
};

module.exports = MDMPlugin;
