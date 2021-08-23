/*
 * This script is meant to be run on Trace Compass with EASE scripting module.
 */

loadModule('/TraceCompass/Trace')
loadModule('/TraceCompass/Analysis');
loadModule('/TraceCompass/DataProvider');
loadModule('/TraceCompass/View');
loadModule('/TraceCompass/Utils');

function runAnalysis(ss) {
    var tasks = {};

    var iter = analysis.getEventIterator();
    while (iter.hasNext()) {
        event = iter.next();
        event_name = event.getName();
        event_ts = event.getTimestamp().toNanos();

        if (event_name == "task_create") {
            task_name = getFieldValue(event, "name");
            task_identifier = getFieldValue(event, "identifier");
            task_priority = getFieldValue(event, "priority");
            task_stack_start = getFieldValue(event, "stack_start");
            task_stack_end = getFieldValue(event, "stack_end");
            task_stack_size = task_stack_end - task_stack_start + 8;

            task = {
                name: task_name,
                identifier: task_identifier,
                priority: task_priority,
                stack_start: task_stack_start,
                stack_end: task_stack_end,
                stack_size: task_stack_size,
                quark: 0,
            }

            task.quark = ss.getQuarkAbsoluteAndAdd('[' + task.priority + '] ' + task.name);
            tasks[task.identifier] = task

            ss.modifyAttribute(event_ts, 0, task.quark);
        }

        if (event_name == "task_switched_out") {
            task_identifier = getFieldValue(event, "identifier");
            task_top_of_stack = getFieldValue(event, "top_of_stack");
            task = tasks[task_identifier];
            task_stack_usage = task.stack_size - (task_top_of_stack - task.stack_start + 8);

            ss.modifyAttribute(event_ts, task_stack_usage, task.quark);
        }
    }

    if (event != null) {
        ss.closeHistory(event.getTimestamp().toNanos());
    }
}

var analysis = getAnalysis("FreeRTOS Stack Usage");
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
provider = createXYProvider(analysis, map);
if (provider != null) {
    openXYChartView(provider);
}