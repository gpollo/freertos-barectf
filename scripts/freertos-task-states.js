/*
 * This script is meant to be run on Trace Compass with EASE scripting module.
 */

loadModule('/TraceCompass/Trace')
loadModule('/TraceCompass/Analysis');
loadModule('/TraceCompass/DataProvider');
loadModule('/TraceCompass/View');
loadModule('/TraceCompass/Utils');

function runAnalysis(ss) {
    var ticker_quark = ss.getQuarkAbsoluteAndAdd('ticker');
    var tasks = {};
    var current_running_task = null;

    var iter = analysis.getEventIterator();
    while (iter.hasNext()) {
        event = iter.next();
        event_name = event.getName();
        event_ts = event.getTimestamp().toNanos();

        if (event_name == "task_increment_tick") {
            ss.modifyAttribute(event_ts - 100e3, "Tick", ticker_quark);
            ss.modifyAttribute(event_ts + 100e3, "", ticker_quark);
        }

        if (event_name == "task_create") {
            task_name = getFieldValue(event, "name");
            task_identifier = getFieldValue(event, "identifier");
            task_priority = getFieldValue(event, "priority");

            task = {
                name: task_name,
                identifier: task_identifier,
                priority: task_priority,
                state: "Ready",
                quark: 0,
            }

            task.quark = ss.getQuarkAbsoluteAndAdd('[' + task.priority + '] ' + task.name);
            tasks[task.identifier] = task

            ss.modifyAttribute(event_ts, task.state, task.quark);
        }

        if (event_name == "moved_task_to_ready_state") {
            task_identifier = getFieldValue(event, "identifier");
            task = tasks[task_identifier];
            task.state = "Ready";

            ss.modifyAttribute(event_ts, task.state, task.quark);
        }

        if (event_name == "task_switched_out") {
            task_identifier = getFieldValue(event, "identifier");
            task = tasks[task_identifier];

            ss.modifyAttribute(event_ts, task.state, task.quark);
        }

        if (event_name == "task_switched_in") {
            task_identifier = getFieldValue(event, "identifier");
            task = tasks[task_identifier];
            current_running_task = task;

            ss.modifyAttribute(event_ts, "Running", task.quark);
        }

        if (event_name == "task_delay_until" || event_name == "task_delay" || event_name == "task_notify_take" || event_name == "task_notify_take_block") {
            task = current_running_task;
            task.state = "Blocked";

            quark = tasks[task.identifier];
            ss.modifyAttribute(event_ts, task.state, task.quark);
        }
    }

    if (event != null) {
        ss.closeHistory(event.getTimestamp().toNanos());
    }
}

var analysis = getAnalysis("FreeRTOS Task States");
if (analysis == null) {
    print("Trace is null");
    exit();
}

var ss = analysis.getStateSystem(false);
if (!ss.waitUntilBuilt(0)) {
    runAnalysis(ss);
}

var map = new java.util.HashMap();
map.put(ENTRY_PATH, '*');
provider = createTimeGraphProvider(analysis, map);
if (provider != null) {
    openTimeGraphView(provider);
}
