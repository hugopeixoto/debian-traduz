class SearchController < ApplicationController
  def index
  end

  def empty
    @matches = Translation.includes(:translation_file).where("msgstr = ''").sort_by { |x| -x.msgid.length }
    render 'search'
  end

  def untranslated
    @matches = Translation.includes(:translation_file).where("msgid = msgstr").sort_by { |x|  -x.msgid.length }
    render 'search'
  end

  def search
    @matches = Translation.includes(:translation_file).where(["msgid LIKE :tag OR msgstr LIKE :tag", tag: "%#{params[:text]}%"])
  end
end
