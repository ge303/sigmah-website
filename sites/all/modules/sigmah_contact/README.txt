2013/16/05  ---sigmah_contact (and related patches)

--- README  ----------------------------------------------------------------------------------------

Just a very simple but UI configurable Drupal 6 module that makes the contact form independant from 
"e-mail fetching" modules (e.g Listhandler) as far as user and node creation are concerned. This module 
can handle these tasks immediately after the contact form is successfully submitted; then it may 
redirect the user back to index or to a landing page with a configurable redirection timer 
(page-contact-submitted.tpl.php), where the operations that have been performed or skipped may be shown 
to him/her.

--- WHAT IT DOES AND SETTINGS -----------------------------------------------------------------------

+ Works with submissions by logged in users and (unregistered users) visitors. It works when the contact 
  page is normally loaded and also when it's embedded in a lightbox.

+ Can create users when visitors submit their message using the form. It disallows
  the form submission until the visitor provides a unique name and email combination.

+ Can create og groups (forum) posts when visitors choose to send their message to a correctly 
  configured and posting enabled contact category.

+ Creates a settings page accessible from its own menu item ('sigmah contact settings') in the 
  Site Configuration menu. The url is: admin/settings/sigmah_contact. The relative permission
  is: administer sigmah_contact. 

  Configurable Options:
  - Every contact form category can be set (or not) as:
    + managed by the module.
    + linked to an og group (forum) for contact form forum posting; all og group (forum) are selectable.
    + hidden from the contact form.

  - Option to redirect the contact form - on successfull submission - directly to index or
    to the .tpl landing page with configurable redirection/popup close timer and custom CSS.

  - Option to override the contact form category management, so that when a category
    is not managed and enabled in this module, it would be hidden from the contact form.

  - User and node creation can be enabled/disabled. When user creation is disabled, the user flood
    limit is ignored.

  - The contact form categories can be hyperlinked, pointing to their associated og group (forum).

  - An hourly flood threshold can be configured and enabled for 'contact form user registration', 
    in case the Drupal hourly contact threshold is higher.

  - The contact form category destination email can be shown to users after a successfull submit.

  - If the user selects a contact form category that is managed and posting enabled but not linked
    to any og group (forum), then the post is inserted without a group and the user is notified 
    that a moderator would move it into the correct forum.

+ Logs notices and errors (watchdog).

+ Full install and unistall support using the schema API. No Drupal SQL tables are modified, just 
  a new (small) table is created.

--- INSTALLATION -----------------------------------------------------------------------------------

This module sets up all its default values and inserts the sigmah.org specific category -> og group 
linking values (three rows) in its DB table, to allow immediate post install sigmah.org functioning. 
Otherwise the module would consider all categories as "non managed" after the installation, requiring
a visit to the settings page to set up the associations.

--- IMPORTANT PATCHES IN THE COMMIT -----------------------------------------------------------------

REQUIRED BY THIS MODULE: The sigmah.module two line patch is required to make the two play nice with 
each other (some form altering code comes from sigmah.module). When this module is active, the patched 
sigmah.module will just perform theming related stuff in the appropriate hook (theming the form submit 
button at the moment).

RECOMMENDED: The sigmah-theme/template.php patch loads the sigma-theme/new_css/pop.css stylesheet for 
the default contact form landing page, which is using the contact form layout.

--- OTHER COMMIT PATCHES ----------------------------------------------------------------------------

The sigmah-theme/pop.css and sigmah-theme/page-contact-tpl.php patches adjust the margin-top of 
messages.error that otherwise partly slides behind the fixed header, and "step 2/2" is now hidden when 
the contact form is not in a lightbox.