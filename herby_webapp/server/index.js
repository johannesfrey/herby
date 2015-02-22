var path = require('path');
var express = require('express');
var app = express();

var apiRoute = require('./routes').api;

app.use('/', express.static(path.join(__dirname, '../client/build')));
app.use('/api', apiRoute);


app.listen(3000);
