/*
- Create 8 reader threads that all acquire the read lock, then stay in the read section for a short time (e.g., a
loop), then release.
- Verify (using an atomic counter) that at some point there were at least 2 readers inside simultaneously
- Verify no deadlock

return 0 on success
return 1 on failure
(Printing a short failure message is recommended.)
*/ 