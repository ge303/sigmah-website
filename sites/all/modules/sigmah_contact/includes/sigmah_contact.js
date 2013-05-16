/**
* sigmah_contact.js
* Contact form submission landing page JS.
*/

(function ($) {
  Drupal.behaviors.redirect_close_timeout = function(context, settings){
      var t = setTimeout(function(){
              redirect_close();
      }, Drupal.settings.sigmah_contact.success_submit_redirect_ms);
  };
})(jQuery);

var redirect_close = function () {
   (parent.$.fancybox !== undefined) ? parent.$.fancybox.close() : 
   location.href = Drupal.settings.basePath;
}
