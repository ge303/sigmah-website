<?php
    $messages = sigmah_set_result(NULL, NULL);
    if(!sigmah_contact_option_get($messages, "submission_success")) { module_invoke_all('exit'); exit; }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    
    <title>Thank you</title><?php /*print $head_title;*/ ?>
    <?php print $head; ?>
    <?php print $styles; ?>
    <?php print $scripts; ?>
  </head>
<body id="page-contact-submitted">
  
  <div class="headerPop">
    </div>
    <div class="contentPop"> 
      <h4><?php echo t('Thanks for your request:'); ?></h4>
<table class="contentTable">
<?php

function template_print_row($success, $text)
{
   global $base_path;
   $src_icon_check  =  $base_path.drupal_get_path('module', 'sigmah_contact')."/images/check.png";
   $src_icon_notice  =  $base_path.drupal_get_path('module', 'sigmah_contact')."/images/notice.png";
   $success ? $img_src = $src_icon_check : $img_src = $src_icon_notice;
   print "<tr><td><img src='$img_src' class='small_icon'/></td><td>$text<td>";
}

$recipient_str = " ".t("us");
if(variable_get("sigmah_contact_show_category_email_to_users", 0) == "1"):
    $recipients = sigmah_contact_option_get($messages, "email_recipients");
    $recipient_str = t(" us at: ").$recipients["data"];
endif;
if(empty($message_link)) $message_link = "message";//forum posting failed...

if(sigmah_contact_option_get($messages, "email_sent_copy_to_user")):
  template_print_row(TRUE, t('A copy of your ').$message_link.t(' has been emailed to yourself and to %recipient_str.', 
               array("%recipient_str" => $recipient_str)));
 
else:
  template_print_row(TRUE, t('A copy of your ').$message_link.t(' has been emailed to %recipient_str.', 
               array("%recipient_str" => $recipient_str)));
  
endif;

$result_user = sigmah_contact_option_get($messages, "user_account_created");
if($result_user["success"] == true):
  $user = $result_user["data"];
  template_print_row(TRUE, t("Your sigmah.org account has been created and is active. Check <i>%mail</i> for the password.", array("%mail" => $user->mail)));
elseif(!user_is_logged_in()):/*no user creation for logged in users*/
  if($result_user["error"] == "create_user_disabled") 
    template_print_row(FALSE, t("We are sorry, but at the moment we cannot automatically register an user from the contact form."));  
  else template_print_row(FALSE,  t("User creation failure: ").$result_user["error"]);  
endif;
$result_forumpost = sigmah_contact_option_get($messages, "forum_post_inserted");

if($result_forumpost["success"] == true)://node created
  $node = sigmah_contact_option_get($messages, "forum_post_inserted")["data"];
  $nid = $node->nid;
  $forum_nid = reset($node->og_groups);

  $message_link = sigmah_contact_template_hlink_from_nid($nid, "message", array("target" => "_blank"));

  if(empty($forum_nid)):
     template_print_row(TRUE, t('Your')." ".$message_link.t("  will be reviewed by a moderator and posted in the appropriate forum."));
  else:
      $forum_link   = sigmah_contact_template_hlink_from_nid($forum_nid, "forum", array("target" => "_blank"));
      template_print_row(TRUE, t('Your')." ".$message_link.t(" has been posted in the requested ").$forum_link.".");
  endif;
else:
    //forum post creation error or forum posting disabled (this section could be improved)
    if($result_forumpost["error"] == "create_post_disabled")
       template_print_row(FALSE, t("We are sorry, but at the moment we cannot automatically post your message to the forums from the contact form."));
    else if($result_forumpost["error"] == "create_post_specific_forum_disabled")
       template_print_row(FALSE, t("We are sorry, but at the moment we cannot post your message to the requested forum from the contact form.")); 
    else if($result_forumpost["error"] == "no_user_to_assign_to")
      template_print_row(FALSE, t("We are sorry, but since at the moment we cannot register your user, no forum post can be created."));
    else  template_print_row(FALSE, t("We are sorry, but at the moment we cannot post your message to the requested forum: ".$result_forumpost["error"])); 
endif;
?>
</table>
<?php
$redirect_s = (variable_get("sigmah_contact_form_submit_redirect_ms", "5000") / 1000);
echo "<br><p><a href='javascript: redirect_close();'>".t("Click here")."</a>".t(" to go back now .. or wait ").$redirect_s.t(" seconds.")."</p>";

sigmah_clear_result();//clear the $_SESSION variable
?> 
<br>
</div>
</body>
</html>