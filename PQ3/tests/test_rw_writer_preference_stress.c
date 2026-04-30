/*
- Create 12 reader threads that repeatedly: acquire_read → short work → release_read.
- Create 4 writer threads that repeatedly: acquire_write → short work → release_write.
- Run for enough iterations (e.g., 5,000 total acquisitions across writers).
- Verify every writer thread successfully acquires the write lock many times (e.g., at least 100 acquisitions
each), i.e., writers do not starve.

return 0 on success
return 1 on failure
(Printing a short failure message is recommended.)
*/