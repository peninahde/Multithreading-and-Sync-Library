/*
- Create 1 writer thread that acquires the write lock and, while holding it, checks that:
    - no readers are inside (a shared atomic readers_in_cs must be 0)
    - no other writer is inside
- In parallel, create 8 reader threads trying to read repeatedly.
- Verify the writer never overlaps with any reader/writer in the critical section.

return 0 on success
return 1 on failure
(Printing a short failure message is recommended.)
*/