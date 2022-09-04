import QtQuick 2.15


Item
{
    id: root
    property var action
    property var signalToBindTo
    property var callbackMethod
    
    function cleanUp()
    {
        if(action())
        {
            signalToBindTo.connect(callbackMethod);
            return false;
        }
        
        return true;
    }
}