<?php
/**
 * sigmah_contact.inc
*/


/**
 * DB helper functions.
*/

function sigmah_contact_user_exists($name, $mail) {
	$dbq = db_query("SELECT * FROM {users} WHERE LOWER(name) = LOWER('%s') OR LOWER(mail) = LOWER('%s')", 
			$name, $mail);
	return ($dbq->num_rows > 0);
}

function cid_og_load() {

	$dbq = "SELECT 
	{og}.*,
	{contact_cid_og}.enabled,
	{contact_cid_og}.forum_posting_disabled,
	{contact}.cid, 
	{contact}.category
	FROM {og}
	LEFT JOIN {contact_cid_og}
	ON {contact_cid_og}.nid = {og}.nid
	LEFT JOIN {contact}
	ON {contact}.cid = {contact_cid_og}.cid";
	$result = db_query($dbq);
	return $result;
}

function cid_og_load_group_by_contact_cid($contact_cid) 
{
  $sql = "SELECT *,{contact_cid_og}.managed  FROM {og} 
          JOIN {contact_cid_og}
          ON {contact_cid_og}.nid = {og}.nid
          WHERE {contact_cid_og}.cid = '%n'";
  $result = db_query($sql, $contact_cid);
  if(empty($result)) return FALSE;
  $node = db_fetch_array($result);
  return $node;
}

function cid_og_can_post_to($options=null) 
{
	$sql = "SELECT 1 FROM {contact_cid_og}
	        WHERE {contact_cid_og}.forum_posting_disabled = 1";
	
	$param = null;
	if(sigmah_contact_option_set($options, "cid")){
		$sql .= " AND {contact_cid_og}.cid = %d"; $param = $options["cid"];
	}
	else if (sigmah_contact_option_set($options, "og_nid")){
		$sql .= " AND {contact_cid_og}.nid = %d"; $param = $options["og_nid"];
	}
	else return FALSE;

	$dbr = db_query($sql, $param);
	return ($dbr->num_rows == 0);
}

function cid_og_loadfilter_cids($options) 
{
	 $sql = "
		SELECT {contact}.cid
		FROM {contact}";
	
	if(sigmah_contact_option_set($options, "filter_unmanaged")) $sql .= " LEFT ";	
	$sql .=	" JOIN {contact_cid_og}
		ON {contact_cid_og}.cid = {contact}.cid";

	if(sigmah_contact_option_set($options, "filter_disabled"))
		$sql .= " WHERE {contact_cid_og}.enabled = 0 AND {contact_cid_og}.managed = 1";

	else if(sigmah_contact_option_set($options, "filter_unmanaged"))
		$sql .= " WHERE {contact_cid_og}.nid IS NULL OR {contact_cid_og}.managed = 0";//unmanaged categories are either set as managed = 0 or not (yet) present in {contact_cid_og}

	$result = db_query($sql, $enabled);
	if(empty($result)) return FALSE;
	$records = array();
	while ($rf = db_fetch_array($result)) {
		array_push($records, $rf["cid"]);
	}
	return $records;
}

function cid_og_load_cids($options=null) 
{
    $sql = "
		SELECT {contact}.*, 
	    {og.nid},
	    {contact_cid_og}.enabled,
	    {contact_cid_og}.forum_posting_disabled,
	    {contact_cid_og}.managed    
		FROM {contact}
		LEFT JOIN {contact_cid_og}
		ON {contact_cid_og}.cid = {contact}.cid
		LEFT JOIN {og}
		ON {og}.nid = {contact_cid_og}.nid";

	$result = db_query($sql, $contact_cid);
	if(empty($result)) return FALSE;
	$records = array();
	while ($rf = db_fetch_array($result)) {
		array_push($records, $rf);
	}
	return $records;
}

/**
 * Misc helpers.
*/

function sigmah_contact_option_set($options, $key, $val=null){	
	if(empty($val)) return !empty($options) && !empty($options[$key]);
	else 		    return !empty($options) && !empty($options[$key]) && $options[$key] == $val;
}

function sigmah_contact_option_get($options, $key, $defaultVal=null){	
	if(empty($options) || empty($key) || empty($options[$key])) return $defaultVal;
	else return $options[$key];
}

function sigmah_contact_template_hlink_from_nid($nid, $text, $options) {
	
 	sigmah_contact_option_set($options, "target") ? $attributes = array("attributes" => array("target" => $options["target"])) : $attributes = null;
 	return l(t($text), "node/".$nid, $attributes);	
}

function form_recursive_attr_set(&$elements, $options){
  foreach (element_children($elements) as $key) {
    if (isset($elements[$key]) && $elements[$key]) {

      // Recurse through all children elements.
      form_recursive_attr_set($elements[$key], $options);
    }
  }

  if(sigmah_contact_option_set($options, "attributes"))
  {
  	  	  foreach($options["attributes"] as $attr)
  	  	  {   
  	  	  	  if(sigmah_contact_option_set($options, "is_attributes_child"))
  	  		  {
				  if (!isset($elements['#attributes'])) {
				    $elements['#attributes'] = array();
				  }
				  $elements['#attributes'][$attr["name"]] = $attr["value"]; 
			  }
			  else
  	  		  {	
  	  		  	  $elements[$attr["name"]] = $attr["value"];
  	  		  }	  
		  }
  }
}

function sigmah_transfer_settings_js() {

  drupal_add_js(array('sigmah_contact' => array(	
  	"success_submit_redirect_ms" => variable_get('sigmah_contact_form_submit_redirect_ms', '5000') 
  	)
  	), 'setting');
}

/**
 * Resets all the module stored settings.
 */
function sigmah_contact_reset_all() {
  db_query("DELETE FROM {variable} WHERE name LIKE 'sigmah_contact\_%'"); //delete variables.
  db_query("DELETE FROM {cache} WHERE cid='variables'"); //clear cache.
  db_query("DELETE FROM {contact_cid_og}"); //delete cid_og linking table.
}
?>