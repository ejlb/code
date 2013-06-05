local http = require "http"
local shortport = require "shortport"
local string = require "string"
local url = require "url"
local json = require "json"
local table = require "table"

description = [[

description
]]

---
--@output
--output

--[[ scripts/meta/env --]]
--[[ pass in app database --]]
--[[ verify with extension --]]
--[[ data location/author permissions --]]

author = "Eddie Bell"
license = "Same as Nmap--See http://nmap.org/book/man-legal.html"
categories = {"discovery", "safe"}

portrule = shortport.http

fingerprint_html = function(fingerprint, response)
  if(not(fingerprint['html'])) then
      return nil
  end
  
  local regex = pcre.new(fingerprint["html"], 0, "C")
  return regex:match(response.body)
end

fingerprint_headers = function(fingerprint, response)
  if(not(fingerprint['headers'])) then
      return nil
  end

  match = true
  for k,v in pairs(fingerprint['headers']) do
      lk = string.lower(k)

      if(not(response.header[lk])) then
          match = false
          break
      end

      local regex = pcre.new(v, 0, "C")

      if(not(regex:match(response.header[lk]))) then
          match = false
          break
      end
  end
  return match
end

fingerprint_meta = function(fingerprint, response)
  if(not(fingerprint['meta'])) then
      return nil
  end
end

fingerprint_script = function(fingerprint, response)
  if(not(fingerprint['script'])) then
      return nil
  end
end

fingerprint_env = function(fingerprint, response)
  if(not(fingerprint['env'])) then
      return nil
  end
end

function fingerprint_implies(table, fingerprint)
  if fingerprint['implies'] == nil then
      return table
  end

  if type(fingerprint['implies']) == 'string' then
      table.insert(found_webapps, fingerprint['implies'])
    else
      for k,v in pairs(fingerprint['implies']) do
        table.insert(found_webapps, v)
    end
  end
end

action = function(host, port)
  local response, redirect_url
  response = http.get(host, port, '/')

  -- check for a redirect
  if response.location then
    redirect_url = response.location[#response.location]
    if response.status and tostring(response.status):match("30%d") then
      return {redirect_url = redirect_url}, ("Did not follow redirect to %s"):format(redirect_url)
    end
  end

  if (not(response.body)) then
    return
  end

  local f = io.open('apps.json', 'r')
  if (not(f)) then
      return false, 'Could not open the webapp fingerprint file'
  end

  local webapp_json = f:read("*all")

  ret, webapps_db = json.parse(webapp_json)
  if(not(ret)) then
      return false, 'Failed to parse the webapp fingerprint file'
  end

  local webapps = webapps_db['apps']
  local categories = webapps_db['categories']

  local fingerprint_functions = {
    fingerprint_html,
    fingerprint_headers,
    fingerprint_meta,
    fingerprint_script,
    fingerprint_env
  }

  local found_webapps = {}

  for _, fingerprinter in pairs(fingerprint_functions) do
      for webapp, fingerprint in pairs(webapps) do
          if(fingerprinter(fingerprint, response)) then
              table.insert(found_webapps, webapp)
              table = fingerprint_implies(fingerprint, table)
          end
      end
  end

  if not(next(found_webapps) == nil) then
      local uniq_found_webapps = {}
      for i,v in pairs(found_webapps) do
        if(uniq_found_webapps[v] == nil) then
            table.insert(uniq_found_webapps,v)
        end
      end
      return table.concat(uniq_found_webapps, ', ')
  end
end

