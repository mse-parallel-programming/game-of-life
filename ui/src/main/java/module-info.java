module ui.main {
    requires javafx.controls;
    requires javafx.fxml;
    requires com.fasterxml.jackson.databind;

    opens ppr to javafx.fxml;
    exports ppr.messages.helper;
    exports ppr.messages;
    exports ppr;
}