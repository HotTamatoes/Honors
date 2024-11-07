#pragma once

using namespace std;

int added;
int total;

void initializeCounters()
{
  added = 0;
  total = 0;
}

void addAdd()
{
  added += 1;
}

void addTotal()
{
  total += 1;
}

int getAdd()
{
  return added;
}

int getSkip()
{
  return total-added;
}

int getTotal()
{
  return total;
}

string getCounters()
{
  int skipped = total - added;
  return "skipped: " + to_string(skipped) + ", added: " + to_string(added) + ", total: " + to_string(total) + " (" + to_string((double)skipped/total) + ", " + to_string((double)added/total) + ")%";
}

