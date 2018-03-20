/*
 * CCCHH Door Information System for UNIX
 * Author: hanez, Haeger
 * Thanks to: atari, beh and eisbaer
 * 
 * License (Beerware License):
 * As long as you retain this notice you can do whatever you want with this 
 * stuff. If we meet some day, and you think this stuff is worth it, you can buy 
 * me a beer in return hanez and all other contributers
 * 
 * TODO:
 *  - Add door open/close date to notification and status
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <curl/curl.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include <json.h>
#if LIBNOTIFY
#include <libnotify/notify.h>
#endif

#include "pesthoernchen.h"

struct string {
  char *ptr;
  size_t len;
};

static char *name      = "Dooris for UNIX";
static char *statusurl = "https://www.hamburg.ccc.de/dooris/status.json";
static char *agent     = "Dooris-for-UNIX/0.42";

// The delay for polling the dooris service. Adjust this before compiling
int delay              = 900000; // ms. aka 15 minutes

bool door_open         = false;
bool old_door_open     = false;

GtkStatusIcon *tray_icon;

bool do_it();
void invoke_notification();

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

void tray_icon_on_click(GtkStatusIcon *status_icon, gpointer user_data) {
  do_it();
}

void get_bouncer_data() {
  CURL *curl;
  CURLcode res;
  
  struct json_object *response_json_object;
  struct json_object *door_json_object;
  struct json_object *leaf_json_object;
  
  bool door_status;
  int door_last_change;

  struct string s;

  printf("Called get_bouncer_data()\n");

  old_door_open  = door_open;

  init_string(&s);
  
  curl = curl_easy_init();
  if(!curl) {
    fprintf(stderr, "curl_easy_init() failed.\n");
    return;
  }

  curl_easy_setopt(curl, CURLOPT_URL, statusurl);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, agent);

  res = curl_easy_perform(curl);

  curl_easy_cleanup(curl);

  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    return;
  }

  response_json_object = json_tokener_parse(s.ptr);
  free(s.ptr);

  // door objects
  door_json_object = json_object_object_get(response_json_object, "state");

  leaf_json_object = json_object_object_get(door_json_object, "open");
  door_status = json_object_get_int(leaf_json_object);
  //door_status = true;
  leaf_json_object = json_object_object_get(door_json_object, "lastchange");
  door_last_change = json_object_get_int(leaf_json_object);

  // free door object
  json_object_put(door_json_object);

  // free leaf object
  //json_object_put(leaf_json_object);
  
  // free response object
 // json_object_put(response_json_object);
  
  printf("Door status: = %d\n", door_status);
  printf("Door last change: = %d\n", door_last_change);

  if (door_status == true) {
    door_open = true;
  } else {
    door_open = false;
  }
}

void invoke_notification() {
  char door[20];
  
  if (door_open == true) {
    snprintf(door, 20, "Door open...");
  } else {
    snprintf(door, 20, "Door closed...");
  }
  
  gtk_status_icon_set_tooltip(tray_icon, door);

#if LIBNOTIFY
  NotifyNotification *n;

  notify_init(name);
  n = notify_notification_new(name, door, NULL);
  //notify_notification_set_timeout(n, 10000);

  if (door_open == true) {
  notify_notification_set_icon_from_pixbuf(n, gdk_pixbuf_from_pixdata(
                                              &icon_yellow_pixbuf, 
                                              true, 
                                              NULL));
  } else {
  notify_notification_set_icon_from_pixbuf(n, gdk_pixbuf_from_pixdata(
                                              &icon_red_pixbuf, 
                                              true, 
                                              NULL));
  }
  notify_notification_show(n, NULL);
  g_object_unref(G_OBJECT(n));
#endif
}

void set_status() {
  if (door_open == true) {
    printf("Set to status: open\n");
    gtk_status_icon_set_from_pixbuf(tray_icon, 
                                    gdk_pixbuf_from_pixdata(&icon_yellow_pixbuf, 
                                    true, 
                                    NULL));
  } else {
    printf("Set to status: closed\n");
    gtk_status_icon_set_from_pixbuf(tray_icon, 
                                    gdk_pixbuf_from_pixdata(&icon_red_pixbuf, 
                                    true, 
                                    NULL));
  } 
}

bool do_it() {  
  get_bouncer_data();
  set_status();

  if (door_open != old_door_open) {
    invoke_notification();
  }
  return true;
}

int main(int argc, char **argv) {
  gtk_init(&argc, &argv);

  tray_icon = gtk_status_icon_new();
  g_signal_connect(G_OBJECT(tray_icon), "activate", 
                   G_CALLBACK(tray_icon_on_click), NULL);

  gtk_status_icon_set_visible(tray_icon, true);
  
  do_it();
 
  // DEBUG STUFF 
  //door_open = true;
  //set_status();
  // END DEBUG STUFF

  invoke_notification();

  gtk_timeout_add(delay, (GtkFunction)do_it, (gpointer)NULL);

  gtk_main();
  
  return 0;
}
