import core
import datetime
import time

class Point(object):
    def __init__(self, owned=True, handle=None):
        if handle:
            self.handle = handle
        else:
            self.handle = core.las.LASPoint_Create()
        self.owned = owned
    def __del__(self):
        if self.owned:
            core.las.LASPoint_Destroy(self.handle)
    
    def get_x(self):
        """Returns the X coordinate of the LAS point. 
        
        You are expected to properly de-scale the point according to the offset
        and the X scale if it is a free-standing point.  When points are 
        written to a LAS file, they will be scaled according to the header 
        parameters
        """
        return core.las.LASPoint_GetX(self.handle)
    def set_x(self, value):
        """Sets the X coordinate of the LAS point to a floating point value."""
        return core.las.LASPoint_SetX(self.handle, value)
    x = property(get_x, set_x)

    def get_y(self):
        """Returns the Y coordinate of the LAS point. 
        
        You are expected to properly de-scale the point according to the offset
        and the Y scale if it is a free-standing point.  When points are 
        written to a LAS file, they will be scaled according to the header 
        parameters
        """
        return core.las.LASPoint_GetY(self.handle)
    def set_y(self, value):
        """Sets the Y coordinate of the LAS point to a floating point value."""
        return core.las.LASPoint_SetY(self.handle, value)
    y = property(get_y, set_y)

    def get_z(self):
        """Returns the Z coordinate of the LAS point. 
        
        You are expected to properly de-scale the point according to the offset
        and the Z scale if it is a free-standing point.  When points are 
        written to a LAS file, they will be scaled according to the header 
        parameters
        """
        return core.las.LASPoint_GetZ(self.handle)
    def set_z(self, value):
        """Sets the Z coordinate of the LAS point to a floating point value."""
        return core.las.LASPoint_SetZ(self.handle, value)
    z = property(get_z, set_z)
    
    def get_return_number(self):
        """Returns the return number of the point"""
        return core.las.LASPoint_GetReturnNumber(self.handle)
    def set_return_number(self, value):
        """Sets the return number of the point to an integer"""
        core.las.LASPoint_SetReturnNumber(self.handle, value)
    return_number = property(get_return_number, set_return_number)
    
    def get_number_of_returns(self):
        """Returns the number of returns for the point"""
        return core.las.LASPoint_GetNumberOfReturns(self.handle)
    def set_number_of_returns(self, value):
        """Sets the number of returns for the point"""
        core.las.LASPoint_SetNumberOfReturns(self.handle, value)
    number_of_returns = property(get_number_of_returns, set_number_of_returns)
    
    def get_scan_direction(self):
        """Returns the scan direction angle for the point"""
        return core.las.LASPoint_GetScanDirection(self.handle)
    def set_scan_direction(self, value):
        """Sets the scan direction angle as an integer for the point"""
        core.las.LASPoint_SetScanDirection(self.handle, value)
    scan_direction = property(get_scan_direction, set_scan_direction)
    
    def get_flightline_edge(self):
        return core.las.LASPoint_GetFlightLineEdge(self.handle)
    def set_flightline_edge(self, value):
        """Sets the flightline edge as an integer for the point"""
        core.las.LASPoint_SetFlightLineEdge(self.handle, value)
    flightline_edge = property(get_flightline_edge, set_flightline_edge)
    
    def get_scan_flags(self):
        return core.las.LASPoint_GetScanFlags(self.handle)
    def set_scan_flags(self, value):
        core.las.LASPoint_SetScanFlags(self.handle, value)
    scan_flags = property(get_scan_flags, set_scan_flags)
    
    def get_classification(self):
        return core.las.LASPoint_GetClassification(self.handle)
    def set_classification(self, value):
        core.las.LASPoint_SetClassification(self.handle, value)
    classification = property(get_classification, set_classification)
    
    def get_scan_angle_rank(self):
        return core.las.LASPoint_GetScanAngleRank(self.handle)
    def set_scan_angle_rank(self, value):
        core.las.LASPoint_SetScanAngleRank(self.handle, value)
    scan_angle = property(get_scan_angle_rank, set_scan_angle_rank)

    def get_user_data(self):
        return core.las.LASPoint_GetUserData(self.handle)
    def set_user_data(self):
        core.las.LASPoint_SetUserData(self.handle, value)
    user_data = property(get_user_data, set_user_data)

    def get_time(self):
        t = core.las.LASPoint_GetTime(self.handle)
        return datetime.datetime(*time.localtime(t)[0:7])
    def set_time(self, value):
        t = time.mktime(value.timetuple())
        core.las.LASPoint_SetTime(self.handle,t)
    time = property(get_time, set_time)