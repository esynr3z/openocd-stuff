# Helper for NRF52 access procedures

# CTRL-AP registers
set CTRLAP_RESET            0x00
set CTRLAP_ERASEALL         0x04
set CTRLAP_ERASEALLSTATUS   0x08
set CTRLAP_APPPROTECTSTATUS 0x0C
set CTRLAP_IDR              0xFC

# ctrlap_reset: trigger soft reset through CTRL-AP
proc ctrlap_reset {} {
    global CTRLAP_RESET

    echo "Soft reset will be performed"
    nrf52.dap apreg 1 $CTRLAP_RESET 0x01
    nrf52.dap apreg 1 $CTRLAP_RESET 0x00
    echo "Done!"
}
add_usage_text ctrlap_reset ""
add_help_text ctrlap_reset "Trigger soft reset through CTRL-AP."

# ctrlap_erase: trigger Flash, UICR and RAM erasing
proc ctrlap_erase {} {
    global CTRLAP_ERASEALL
    global CTRLAP_ERASEALLSTATUS

    echo "Flash, UICR and RAM will be erased"
    nrf52.dap apreg 1 $CTRLAP_ERASEALL 0x01
    while {[capture "nrf52.dap apreg 1 $CTRLAP_ERASEALLSTATUS"] == 1} {
        sleep 10
    }
    echo "Done!"
}
add_usage_text ctrlap_erase ""
add_help_text ctrlap_erase "Trigger Flash, UICR and RAM erase through CTRL-AP."

# ctrlap_unlock: unlock debug access port (Flash, UICR and RAM will be erased)
proc ctrlap_unlock {} {
    global CTRLAP_APPPROTECTSTATUS

    ctrlap_erase
    ctrlap_reset
    if {[capture "nrf52.dap apreg 1 $CTRLAP_APPPROTECTSTATUS"] == 0} {
        echo "Error! Access port protection still enabled!"
    } else {
        echo "Protection is disabled!"
    }
}
add_usage_text ctrlap_unlock ""
add_help_text ctrlap_unlock "Unlock debug access port (Flash, UICR and RAM will be erased)."
