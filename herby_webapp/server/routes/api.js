var _ = require('underscore');
var router = require('express').Router();
var params = require('express-params');
var jsonParser = require('body-parser').json();
var i2c = require('i2c');
var address = 0x08;
var wire = new i2c(address, {device: '/dev/i2c-1'});

var herbyProtocolStartByte = 0xc5;
var herbyProtocolStartBytes = [ 0x08, 0x10 ];
var herbyProtocolEndBytes = [ 0x00, 0x00 ];

params.extend(router);

var dummyStats = {
  '00': 'Dry',
  '01': 'Moist',
  '02': 'Dry',
  '10': 'Moist',
  '11': 'Dry',
  '12': 'Moist',
  '20': 'Dry',
  '21': 'Moist',
  '22': 'Dry',
  '30': 'Moist',
  '31': 'Dry',
  '32': 'Moist'
};

function addPosToArray(pos) {
  var dummyCache = [];
  var parity = herbyProtocolStartByte;
  var stepper = pos.slice(0,1);
  var dc = pos.slice(1,2);
  stepper = +stepper;
  dc = +dc;

  dummyCache.push(herbyProtocolStartBytes, stepper, dc, herbyProtocolEndBytes); 
  merged = _.flatten(dummyCache);
  merged.forEach(function(el) {
    parity ^= el;
  });
  merged.push(parity);

  return merged;
}


router.use(function (req, res, next) {
  console.log('%s %s %s', req.method, req.url);
  next();
});


router.route('/')
.all(function(req, res, next) {
  res.redirect('/');
});

router.param('cell', /^[0-3][0-2]$/);
router.route('/cells/:cell')
.get(function(req, res, next) {
  res.status(200).json({ stats: dummyStats[req.params.cell[0]] });
})
.post(jsonParser, function(req, res, next) {
  if (!req.body) return res.sendStatus(400);
  if (req.body.doWork) {
    var pos = req.params.cell[0];
    var data = addPosToArray(pos);

    wire.writeBytes(herbyProtocolStartByte, data , function(err) {
      if (err) {
	console.log(err);
        return res.status(500).send("I2C writeByte Error");
      }
      return res.status(200).send("I2C Success");
    });
  }
});

module.exports = router;
