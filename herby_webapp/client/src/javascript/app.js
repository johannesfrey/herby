var $ = require('jquery');
var foundation = require('foundation');

var HERBY_API_URL = 'http://192.168.178.121:3000/api';

$(document).foundation();

// initialization: grab window height and
// all important elements from the grid
var viewPortHeight = calcViewPortHeight();
var dcPos = 0;
var stepperPos = 0;

var $herbyGrid = $('.herby-grid');
var $herbyRows = $herbyGrid.find('.herby-grid-row');
var $herbyCells = $herbyGrid.find('.herby-grid-cell');
var $herbyActionsBox = $('.herby-actions-box');
var $herbyStatsActionsModal = $('#stats-actions-modal');
var $herbyLatestStats = $herbyStatsActionsModal.find('.latest-stats');
var $herbyActionsExecuteButton = $('#herby-actions-button');

// Sets position values for stepper and DC motors according
// to the individual position (index) of the cell in the grid
$herbyRows.each(function(indexRow, row) {
  var $cells = $(row).find('.herby-grid-cell');
  $cells.each(function(indexCell, cell) {
    $(cell).data("position", { stepperMotor: indexRow, dcMotor: indexCell });
  });
});

setCellsHeight($herbyCells, 4, viewPortHeight);


// When the window gets resized, sets the min-height
// of the grid cells by dividing the view port height
// by the number of cells.
$(window).on('resize', function onResize() {
  setCellsHeight($herbyCells, 4, calcViewPortHeight());
});

// When we tap on a cell, we extract its grid position and update
// the position of the stepper and the dc with these values
$herbyGrid.on('click', function onGridClick(event) {
  var $cell = $(event.target);
  var cellPosition = $cell.data().position;
  stepperPos = cellPosition.stepperMotor;
  dcPos = cellPosition.dcMotor;

  $herbyStatsActionsModal.foundation('reveal', 'open');
  event.stopPropagation();
});

// When the reveal modal opens we perform an AJAX GET request
// in order to receive the latest stats for the plant on that
// particular grid position
$herbyStatsActionsModal.on('opened', herbyAjax('GET', function(stats) {
  $herbyLatestStats.html('<p> Latest Stats: ' + stats.stats + '</>');
}));

// When we click the 'Execute' button in the modal, we perform
// an AJAX POST request, which delegates "Herby" to do its work
// for the plant on the particular grid position.
//
// TODO: Send over actual activities to perform
$herbyActionsExecuteButton.on('click', herbyAjax('POST', {'doWork': true}, function(data) {
  alert(data);
  $herbyStatsActionsModal.foundation('reveal', 'close');
}));




function calcViewPortHeight() {
  return ($(window).height() - 85);
}

function setCellsHeight($cells, number, height) {
  $cells.css('min-height', height/number);
}

function getCheckedActions($selector) {
  var checkedActions = $($selector + ':checked')
                        .map(function() { return this.value })
                        .get();
  return checkedActions;
}

function herbyAjax(type, data, success) {
  if (typeof data === 'function') {
    success = data;
    data = {};
  }
  return function () {
    $.ajax({
      type: type,
      contentType: 'application/json',
      data: JSON.stringify(data),
      url: HERBY_API_URL + "/cells/" + stepperPos + dcPos,
      success: success,
      error: function() {
        alert("Error");
      }
    });
  }
}
