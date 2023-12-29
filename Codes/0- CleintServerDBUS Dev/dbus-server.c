#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

int main(int argc, char **argv) {
    DBusError error;
    DBusConnection *connection;
    int ret;

    dbus_error_init(&error);

    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Connection Error (%s)\n", error.message);
        dbus_error_free(&error);
        return 1;
    }

    ret = dbus_bus_request_name(connection, "com.example.TestServer", DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Name Error (%s)\n", error.message);
        dbus_error_free(&error);
        return 1;
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        return 1;
    }

    while (1) {
        DBusMessage *message;

        dbus_connection_read_write(connection, 0);
        message = dbus_connection_pop_message(connection);

        if (message == NULL) {
            continue;
        }

        if (dbus_message_is_method_call(message, "com.example.TestServer", "HelloWorld")) {
            DBusMessage *reply;

            reply = dbus_message_new_method_return(message);
            dbus_message_append_args(reply, DBUS_TYPE_STRING, &"Hello, World!", DBUS_TYPE_INVALID);
            dbus_connection_send(connection, reply, NULL);
            dbus_message_unref(reply);
        }

        dbus_message_unref(message);
    }

    return 0;
}
