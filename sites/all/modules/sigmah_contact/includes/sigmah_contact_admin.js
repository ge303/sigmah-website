/**
* sigmah_contact_admin.js
* Module settings form JS.
*/

  (function ($) {
	  Drupal.behaviors.fixset_setting_not_active_labels = function(context, settings){
	      $(".setting-not-active", context).each(function(){/*drupal 6 api won't let you addClass to labels?*/
	              $(this).parent("label").addClass("setting-not-active");
	      });


	  };
  })(jQuery);

  function checkbox_category_managed_clicked(sender){
  	  var data_cid = jQuery(sender).attr("data-cid");
  	  var checked  = jQuery(sender).is(":checked");
  	  var sel      = "input[data-cid='"+data_cid+"'],select[data-cid='"+data_cid+"']";
  	  
  	  if(checked)
  	  		jQuery(sel).removeClass("setting-not-active").parent("label").removeClass("setting-not-active");
  	  else  jQuery(sel).addClass("setting-not-active").parent("label").addClass("setting-not-active");
  };  

  function handle_connected(sender, connected_name, invert){
  	  var checked  = jQuery(sender).is(":checked");
  	  var sel      = "[name='"+connected_name+"']";

  	  if((invert && checked) || (!invert && !checked))
  	  		jQuery(sel).addClass("setting-not-active").parent("label").addClass("setting-not-active"); 	  	
  	  else  jQuery(sel).removeClass("setting-not-active").parent("label").removeClass("setting-not-active");
  	  
  };