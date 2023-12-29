#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

int main(int argc, char **argv) {
    DBusError error;
    DBusConnection *conn;
    DBusMessage *msg;
    DBusMessageIter args;
    DBusPendingCall *pending;
    int ret;

    dbus_error_init(&error);

    // Connect to the D-Bus system bus
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Error connecting to the D-Bus system bus: %s\n", error.message);
        dbus_error_free(&error);
        exit(1);
    }

    // Create a new method call message
    msg = dbus_message_new_method_call("com.example.TestServer", "/com/example/TestObject", "com.example.TestInterface", "TestMethod");
    if (msg == NULL) {
        fprintf(stderr, "Error creating D-Bus message\n");
        exit(1);
    }

    // Send the method call message and wait for a reply
    ret = dbus_connection_send_with_reply(conn, msg, &pending, -1);
    if (ret == DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        dbus_error_free(&error);
        exit(1);
    }
    if (!pending) {
        fprintf(stderr, "Error sending D-Bus message\n");
        exit(1);
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    // Wait for the reply
    dbus_pending_call_block(pending);
    msg = dbus_pending_call_steal_reply(pending);
    if (msg == NULL) {
        fprintf(stderr, "Error receiving D-Bus reply\n");
        exit(1);
    }

    // Parse the reply message
    if (!dbus_message_iter_init(msg, &args)) {
        fprintf(stderr, "Error parsing D-Bus reply\n");
        exit(1);
    }
    printf("Received D-Bus reply\n");

    dbus_message_unref(msg);
    dbus_pending_call_unref(pending);

    return 0;
}
