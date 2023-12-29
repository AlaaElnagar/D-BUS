#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

int main(int argc, char **argv) {
    DBusError error;
    DBusConnection *conn;
    DBusMessage *msg;
    DBusMessageIter args;
    int ret;

    dbus_error_init(&error);

    // Connect to the D-Bus system bus
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Error connecting to the D-Bus system bus: %s\n", error.message);
        dbus_error_free(&error);
        exit(1);
    }

    // Request a bus name for our server
    ret = dbus_bus_request_name(conn, "com.example.TestServer", DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Error requesting bus name: %s\n", error.message);
        dbus_error_free(&error);
        exit(1);
    }
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf(stderr, "Error requesting bus name: not primary owner\n");
        exit(1);
    }

    // Register a method with the D-Bus server
    dbus_bus_add_match(conn, "type='method_call',interface='com.example.TestInterface'", &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Error adding D-Bus match rule: %s\n", error.message);
        dbus_error_free(&error);
        exit(1);
    }

    while (1) {
        // Wait for incoming messages
        dbus_connection_read_write(conn, 0);
        msg = dbus_connection_pop_message(conn);

        if (msg == NULL) {
            continue;
        }

        if (dbus_message_is_method_call(msg, "com.example.TestInterface", "TestMethod")) {
            // Handle the method call
            dbus_message_iter_init(msg, &args);
            printf("Received method call\n");

            // Send a reply
            DBusMessage *reply = dbus_message_new_method_return(msg);
            dbus_connection_send(conn, reply, NULL);
            dbus_message_unref(reply);
        }

        dbus_message_unref(msg);
    }

    return 0;
}
